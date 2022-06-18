#include <sstream>
#include <chrono>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "opencv2/opencv.hpp"
#include "yolov5.hpp"
#include "info_struct.hpp"
#include "getTime.hpp"
#include "global.hpp"

using namespace std;

void fire(oatpp::Object<::task_service::dto::AlgoDto> json_data)
{
    float conf = json_data->conf;
    float obj = json_data->obj;
    float iou = json_data->iou;
    int dev_id = json_data->dev_id;
    int camera_id = json_data->camera_id;

    struct stat buffer; 
    std::string bmodel_file = "/data/uestc/ai/model/fire.bmodel";  
    if (stat(bmodel_file.c_str(), &buffer) == 0) {
        OATPP_LOGE("%s", "Cannot find valid model file.");
        exit(1);
    }
    std::string coco_names = "/data/uestc/ai/names/fire.names";
    if (stat(coco_names.c_str(), &buffer) == 0) {
        OATPP_LOGE("%s", "Cannot find classnames file.");
        exit(1);
    }

    BMNNHandlePtr handle = std::make_shared<BMNNHandle>(dev_id);
    std::shared_ptr<BMNNContext> bm_ctx = std::make_shared<BMNNContext>(handle, bmodel_file.c_str());
    YoloV5 yolo(bm_ctx);
    CV_Assert(0 == yolo.Init(conf, obj, iou, coco_names));

    int frame_count=0;
    bool flag = false;
    int img_num = 0;
    
    while(true) 
    {
        shared_ptr<cv::Mat> img = taskqueues[json_data.m_type]->take();
        std::vector<cv::Mat> images{*img};
        std::vector<YoloV5BoxVec> boxes;
        CV_Assert(0 == yolo.Detect(images, boxes));
        for (int i = 0; i < (int) images.size(); ++i) 
        {
            cv::Mat frame = images[i];
            int w = frame.cols;
            int h = frame.rows;
            OATPP_LOGD("frame #'%d': detect boxes: '%d'", frame_count, boxes[i].size());
            int box_len = boxes[i].size();
            if (box_len){
                flag = true;
            }
            for (auto bbox : boxes[i]) 
            {
                if (bbox.x <= 0)
                    bbox.x = 1;
                else if(bbox.x >=w)
                    bbox.x = w-1;
                if (bbox.y <= 0)
                    bbox.y = 1;
                else if(bbox.y >=h)
                    bbox.y = h-1;
                if(bbox.x + bbox.width >=w)
                    bbox.width = w - bbox.x - 2;
                if(bbox.y+bbox.height >=h)
                    bbox.height = h- bbox.y - 2;
                cv::Rect rect(bbox.x+1, bbox.y+1, bbox.width-1, bbox.height-1);
                cv::Mat image_ROI = frame(rect);
                // warning image
                std::string bj_file_img = cv::format("/data/uestc/events/bj_img/%d-%d.jpg", frame_count, img_num);
                cv::imwrite(bj_file_img, image_ROI);
                img_num++;
            }
        img_num = 0;
        }
    frame_count ++;
    }
}
