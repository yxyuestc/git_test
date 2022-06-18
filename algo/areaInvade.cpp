#include "opencv2/opencv.hpp"
#include "yolov5.hpp"
#include "parseMulti.hpp"
#include "judgements.hpp"
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <vector>

#include "bmcv_api_ext.h"
#include "bmlib_utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "info_struct.hpp"
#include <curl/curl.h>
#include "getTime.hpp"
#include "global.hpp"
#include <unistd.h>

using namespace std;
using namespace cv;


void areaInvade(oatpp::Object<::task_service::dto::AlgoDto> json_data)

{
  float conf = json_data->conf;
  float obj = json_data->obj;
  float iou = json_data->iou;
  int dev_id = json_data->dev_id;
  std::string point_list = json_data->point_list;
  int camera_id = json_data->camera_id;

  struct stat buffer;
  std::vector<cv::Point> points;
  std::vector<std::vector<int>> datas;
  parseMulti(point_list, datas);
  for (size_t i=0; i< datas.size(); ++i){
    points.push_back(cv::Point(datas[i][0],datas[i][1]));
  }

  std::string bmodel_file = "/data/uestc/ai/model/areaInvade.bmodel";
  if (stat(bmodel_file.c_str(), &buffer) == 0) {
    OATPP_LOGE("%s", "Cannot find valid model file.");;
    exit(1);
  }

  std::string coco_names = "/data/uestc/ai/names/areaInvade.names";
  if (stat(coco_names.c_str(), &buffer) == 0) {
    OATPP_LOGE("%s", "Cannot find classnames file.");
    exit(1);
  }

  BMNNHandlePtr handle = std::make_shared<BMNNHandle>(dev_id);
  std::shared_ptr<BMNNContext> bm_ctx = std::make_shared<BMNNContext>(handle, bmodel_file.c_str());
  YoloV5 yolo(bm_ctx);
  CV_Assert(0 == yolo.Init(
        conf,
        obj,
        iou,
        coco_names));

  int frame_count=0;
  int img_num =0;

  while(1) 
  {
    bool flag=true;
    shared_ptr<cv::Mat> img = taskqueues[json_data.m_type]->take();
    std:: cout << taskqueues[json_data.m_type]->size() << std::endl;

    std::vector<cv::Mat> images{*img};
    std::vector<YoloV5BoxVec> boxes;
    CV_Assert(0 == yolo.Detect(images, boxes));
    for (int i = 0; i < (int) images.size(); ++i) 
    {
      cv::Mat frame = images[i];
      int w = frame.cols;
      int h = frame.rows;
      for (size_t i=0; i< datas.size(); ++i)
      {
        if (i == datas.size()-1){
          cv::line(frame, cv::Point(datas[i][0],datas[i][1]), cv::Point(datas[0][0],datas[0][1]), cv::Scalar(255, 0, 255), 3, 4);
        }
        else{
          cv::line(frame, cv::Point(datas[i][0],datas[i][1]), cv::Point(datas[i+1][0],datas[i+1][1]), cv::Scalar(255, 0, 255), 3, 4);
        }
      }
      
      OATPP_LOGD("frame #'%d': detect boxes: '%d'", frame_count, boxes[i].size());

      for (auto bbox : boxes[i]) 
      {
        if (judgeIrregular(bbox, points) || judgetoregular(bbox, points)){
          flag = true;
          std::cout << "Access Area" << std::endl;

          if (bbox.x <= 0)
              bbox.x = 1;
          else if(bbox.x >=w)
              bbox.x = w-1;

          if (bbox.y <= 0)
              bbox.y = 1;
          else if(bbox.y >=h)
              bbox.y = h-1;

          if(bbox.x + bbox.width >=w){
              bbox.width = w - bbox.x - 2;
          }

          if(bbox.y+bbox.height >=h){
              bbox.height = h- bbox.y - 2;
          }
          cv::Rect rect(bbox.x, bbox.y, bbox.width, bbox.height);
          cv::Mat image_ROI = frame(rect);
          std::string bj_file_img = cv::format("/data/uestc/events/bj_img/%d-%d.jpg", frame_count, img_num);
          cv::imwrite(bj_file_img, image_ROI);
          strJson += bj_file_img + ",";
          img_num++;
          
          yolo.drawPred_red(bbox.class_id, bbox.score, bbox.x, bbox.y, bbox.x + bbox.width,bbox.y + bbox.height, frame);
        }
        else{
          yolo.drawPred_green(bbox.class_id, bbox.score, bbox.x, bbox.y, bbox.x + bbox.width,bbox.y + bbox.height, frame);
        }
      }

      if(flag)
      {
        std::string output_file = cv::format("/data/uestc/events/output/%d.jpg", frame_count);
        cv::imwrite(output_file, frame);

        strJson += output_file + "\",";
        strJson += "\"alarnImg\" : \"";
        strJson += output_file + "\",";
        strJson += "\"alarnTime\" : \"";
        strJson += timeNow_get() + "\"";
        strJson += "}"; 
        strcpy(szJsonData, strJson.c_str());

        CURL *curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if(curl) 
        {
            curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8098/warning/areaInvade");
            /* Now specify the POST data */
            struct curl_slist *plist = nullptr;
            plist = curl_slist_append(plist, "Content-Type:application/json;charset=UTF-8"); 
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist); 
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szJsonData);
            std::cout << szJsonData << std::endl;

            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
      }
    }
  frame_count ++;
  }
  time_stamp_t base_time = time_point_cast<microseconds>(steady_clock::now());
  ts.calbr_basetime(base_time);
  ts.build_timeline("YoloV5");
  ts.show_summary("YoloV5 Demo");
  ts.clear();
}
