#include "opencv2/opencv.hpp"
#include "yolov5.hpp"
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
#include<unistd.h>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
using namespace std;
using namespace cv;


void personNumber(struct video_info json_data)
{
  float conf = json_data.m_conf;
  float obj = json_data.m_obj;
  float iou = json_data.m_iou;
  int dev_id = json_data.m_dev_id;
  int camera_id = json_data.m_camera_id;
  int person_number_thread = json_data.m_person_number_thread;

  std::string bmodel_file = "/data/uestc/ai/model/personNumber.bmodel";
  if (!fs::exists(bmodel_file)) {
    std::cout << "Cannot find valid model file." << std::endl;
    exit(1);
  }

  std::string coco_names = "/data/uestc/ai/names/personNumber.names";
  if (!fs::exists(coco_names)) {
    std::cout << "Cannot find classnames file." << std::endl;
    exit(1);
  }

  // profiling
  TimeStamp ts;
  BMNNHandlePtr handle = std::make_shared<BMNNHandle>(dev_id);
  // Load bmodel
  std::shared_ptr<BMNNContext> bm_ctx = std::make_shared<BMNNContext>(handle, bmodel_file.c_str());
  YoloV5 yolo(bm_ctx);
  yolo.enableProfile(&ts);
  CV_Assert(0 == yolo.Init(
        conf,
        obj,
        iou,
        coco_names));

  int frame_count=0;
  cv::Mat save_frame;
  while(1) 
  {
      bool flag = false;
      char szJsonData[1024];  
      memset(szJsonData, 0, sizeof(szJsonData));
      std::string strJson = "{";  
      strJson += "\"orgCode\" : \"123456\",";  
      strJson += "\"licence\" : \"MTIzNDU2MTY0NzM5NTkyMzk2NQ==\",";  
      strJson += "\"alarmCode\" : \"13\",";  
      strJson += "\"alarnRemark\" : \"Person Number Thread Warning\",";  
      strJson += "\"videoName\" : \"";
      strJson += to_string(camera_id) + "\",";
      strJson += "\"alarnsWarnImg\" : \""; 

      shared_ptr<cv::Mat> img = taskqueues[json_data.m_type]->take();
      std:: cout << taskqueues[json_data.m_type]->size() << std::endl;

      std::vector<cv::Mat> images{*img};
      std::vector<YoloV5BoxVec> boxes;
      CV_Assert(0 == yolo.Detect(images, boxes));
      for (int i = 0; i < (int) images.size(); ++i) 
      {
        cv::Mat frame = images[i];
        std::cout << "frame #"<<frame_count<<": detect boxes: " << boxes[i].size() << std::endl;
        int box_len = boxes[i].size();
        if (box_len > person_number_thread)
          flag = true;

        if(flag)
        {
          for (auto bbox : boxes[i]) 
            yolo.drawPred_red(bbox.class_id, bbox.score, bbox.x, bbox.y, bbox.x + bbox.width,bbox.y + bbox.height, frame);

          std::string bj_img_file = cv::format("/data/uestc/events/bj_img/%d.jpg", frame_count);
          std::string output_file = cv::format("/data/uestc/events/output/%d.jpg", frame_count);
          cv::resize(frame, save_frame, cv::Size(480,270));
          cv::imwrite(bj_img_file, save_frame);
          cv::imwrite(output_file, frame);

          strJson += bj_img_file + "\",";
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
              curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8098/warning/pnThread");
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
