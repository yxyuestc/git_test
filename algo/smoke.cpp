#include "opencv2/opencv.hpp"
#include "yolov5.hpp"
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <chrono>
#include "info_struct.hpp"
#include "getTime.hpp"
#include <curl/curl.h>
#include "global.hpp"
#include<thread>
#include "nlohmann/json.hpp"
#include<unistd.h>
using json = nlohmann::json;

namespace fs = boost::filesystem;
using namespace std;


void smoke(struct video_info json_data)
{
    float conf = json_data.m_conf;
    float obj = json_data.m_obj;
    float iou = json_data.m_iou;
    int dev_id = json_data.m_dev_id;
    int camera_id = json_data.m_camera_id;

    std::string bmodel_file = "/data/uestc/ai/model/smoke.bmodel";
    if (!fs::exists(bmodel_file)) {
    std::cout << "Cannot find valid model file." << std::endl;
    exit(1);
    }

    std::string coco_names = "/data/uestc/ai/names/smoke.names";
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
    bool flag = false;
    int img_num =0;

    while(1) 
    {
        pid_t pid = getpid();
        cout << "pid=" << pid <<endl;

        char szJsonData[1024];  
        memset(szJsonData, 0, sizeof(szJsonData));
        std::string strJson = "{";  
        strJson += "\"orgCode\" : \"123456\",";  
        strJson += "\"licence\" : \"MTIzNDU2MTY0NzM5NTkyMzk2NQ==\",";  
        strJson += "\"alarmCode\" : \"17\",";  
        strJson += "\"alarnRemark\" : \"Smoke Warning\",";  
        strJson += "\"videoName\" : \"";
        strJson += to_string(camera_id) + "\",";
        strJson += "\"alarnsWarnImg\" : \""; 

        shared_ptr<cv::Mat> img = taskqueues[json_data.m_type]->take();
        std::vector<cv::Mat> images{*img};

        std::vector<YoloV5BoxVec> boxes;
        CV_Assert(0 == yolo.Detect(images, boxes));
        for (int i = 0; i < (int) images.size(); ++i) 
        {
            cv::Mat frame = images[i];
            int w = frame.cols;
            int h = frame.rows;            
            std::cout << "frame #"<<frame_count<<": detect boxes: " << boxes[i].size() << std::endl;
            int box_len = boxes[i].size();
            if (box_len){
                flag = true;
            }
            for (auto bbox : boxes[i]) 
            {
                int left = bbox.x - 2*bbox.width;
                int right = bbox.y - 2*bbox.height;
                int wid = 5*bbox.width;
                int hei = 5*bbox.height;
                if (left <= 0)
                    left = 1;
                else if(left >=w)
                    left = w-1;
                if (right <= 0)
                    right = 1;
                else if(right >=h)
                    right = h-1;
                if(left+wid >=w)
                    wid = w-left-2;
                if(right+hei >=h)
                    hei = h-right-2;
                cv::Rect rect(left, right, wid, hei);
                cv::Mat image_ROI = frame(rect);
                // warning image
                std::string bj_file_img = cv::format("/data/uestc/events/bj_img/%d-%d.jpg", frame_count, img_num);
                cv::imwrite(bj_file_img, image_ROI);
                if (img_num != (box_len-1)){
                    strJson += bj_file_img + ",";
                }
                else{
                    strJson += bj_file_img + "\",";
                }
                img_num++;
            }

            if (flag)
            {
                for (auto bbox : boxes[i]){
                    yolo.drawPred_red(bbox.class_id, bbox.score, bbox.x, bbox.y, bbox.x + bbox.width, bbox.y + bbox.height, frame);
                }
                std::string output_file = cv::format("/data/uestc/events/output/%d.jpg", frame_count);
                cv::imwrite(output_file, frame);

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
                    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8098/warning/smoke");
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
                flag = false;
            }
            img_num = 0;
            }
        frame_count ++;
    }
  time_stamp_t base_time = time_point_cast<microseconds>(steady_clock::now());
  ts.calbr_basetime(base_time);
  ts.build_timeline("YoloV5");
  ts.show_summary("YoloV5 Demo");
  ts.clear();
}
