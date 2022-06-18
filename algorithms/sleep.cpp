#include "opencv2/opencv.hpp"
#include <vector>
#include <numeric>
#include <math.h>
#include "yolov5.hpp"
#include <boost/filesystem.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <chrono>
#include "info_struct.hpp"
#include <curl/curl.h>
#include "getTime.hpp"
#include "global.hpp"

#define PI 3.14159

// namespace fs = boost::filesystem;
using namespace std;


struct center_point {
	int x;
	int y;
	int width;
	int height;
};

double Var(const std::vector<std::map<int, center_point>> &somePoints, int obj_id)
{
	auto func = [&] (int x_or_y) 
	{
		int count = 0;
		double sum = 0.0;
		for (size_t i =0; i< somePoints.size();i++)
		{
			auto it = somePoints[i].find(obj_id);
			if(it != somePoints[i].end())
			{
				count++;
				sum += x_or_y > 0? it->second.y : it->second.x;
			}
		}
		double mean = sum / count;
		double variance = 0.0;
		for (size_t i =0; i< somePoints.size();i++)
		{
			auto it = somePoints[i].find(obj_id);
			if(it != somePoints[i].end())
			{
				variance += x_or_y > 0? pow(it->second.y - mean, 2) : pow(it->second.x - mean, 2);
			}
		}
		return variance / count;
	};
	return func(0) + func(1);
}


double funcNormal(double x, double y, double u1, double u2, double var1, double var2, double r)
{
	double f1 = (1 / (2 * PI * var1 * var2 * pow((1 - pow(r, 2)),0.5)));
	double f2 = -1 * (1 / (2 * (1 - pow(r, 2))));
	double f3 = pow(x - u1, 2) / pow(var1, 2);
	double f4 = 2 * r * (x - u1) * (y - u2) / var1 / var2;
	double f5 = pow(y - u2, 2) / pow(var2, 2);
	return f1 * exp(f2 * (f3 - f4 + f5));
}



// void sleep1(struct video_info json_data)
void sleep1(oatpp::Object<::task_service::dto::AlgoDto> json_data)
{
    float var = json_data->var;
    float conf = json_data->conf;
    float obj = json_data->obj;
    float iou = json_data->iou;
    int dev_id = json_data->dev_id;
    int jg = json_data->jg_time;
    int camera_id = json_data->camera_id;

    std::string bmodel_file = "/data/uestc/ai/model/sleep.bmodel";
    if (stat(bmodel_file.c_str(), &buffer) == 0) {
        std::cout << "Cannot find valid model file." << std::endl;
        exit(1);
    }

    std::string coco_names = "/data/uestc/ai/names/sleep.names";
    if (stat(coco_names.c_str(), &buffer) == 0) {
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
    int ids  = 0;
    std::vector<std::map<int, center_point>> someIdPoints;
    cv::Mat save_frame;

    while(1) 
    {
        bool bj_flag = false;
        char szJsonData[1024];  
        memset(szJsonData, 0, sizeof(szJsonData));
        std::string strJson = "{";  
        strJson += "\"orgCode\" : \"123456\",";  
        strJson += "\"licence\" : \"MTIzNDU2MTY0NzM5NTkyMzk2NQ==\",";  
        strJson += "\"alarmCode\" : \"12\",";  
        strJson += "\"alarnRemark\" : \"Sleep Warning\",";  
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
            std::cout << "frame #"<<frame_count<<": detect boxes: " << boxes[i].size() << std::endl;
            std::map<int, center_point> id_pos;
            std::map<int, YoloV5Box> id_box;
            for (auto bbox : boxes[i])
            {
                if(frame_count == 0)
                {
                    id_pos[ids++] = {bbox.x + bbox.width / 2, bbox.y + bbox.height / 2, bbox.width, bbox.height};
                }
                else
                {
                    std::map<int, center_point> pre_frame_info(someIdPoints.back());
                    double _max = -1;
                    int _max_idx = -1;
                    
                    bool flag = false;
                    for(auto it = pre_frame_info.begin();it != pre_frame_info.end();++it)
                    {
                        double ret = funcNormal(bbox.x + bbox.width / 2 - it->second.x,bbox.y + bbox.height / 2 - it->second.y, 0, 0, 1, 1, 0);
                        if(id_pos.find(it->first) == id_pos.end() && ret > _max)
                        // it->second.y + it->second.width / 2 - it->second.y, 0, 0, 1, 1, 0)
                        {
                            flag = true;
                            _max = ret;
                            _max_idx = it->first;
                        }
                    }
                    if(!flag) id_pos[ids] = {bbox.x + bbox.width / 2, bbox.y + bbox.height / 2, bbox.width, bbox.height};
                    else id_pos[_max_idx] = {bbox.x + bbox.width / 2, bbox.y + bbox.height / 2, bbox.width, bbox.height};
                    if(frame_count % jg == 0)
                    {
                        if(!flag) id_box[ids++] = bbox;
                        else id_box[_max_idx] = bbox;
                    }
                    if(!flag) ids++;
                }

            }
            someIdPoints.push_back(id_pos);

            if (frame_count % jg == 0)
            {
                for(auto it = id_pos.begin();it != id_pos.end();++it)
                {
                    if(Var(someIdPoints, it->first) < var) 
                    {
                        bj_flag = true;
                        std::cout << Var(someIdPoints, it->first) << std::endl;
                        std::cout << "ShuiGang" << std::endl;
                        yolo.drawPred_red(id_box[it->first].class_id, id_box[it->first].score, id_box[it->first].x, id_box[it->first].y, id_box[it->first].x + id_box[it->first].width, id_box[it->first].y + id_box[it->first].height, frame);
                    }
                    else 
                    {
                        std::cout << Var(someIdPoints, it->first) << std::endl;
                        std::cout << "no-ShuiGang" << std::endl;
                    }
                }   
                someIdPoints.clear();
                someIdPoints.push_back(id_pos);
            
            if(bj_flag)
            {
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
                    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8098/warning/sleep");
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
        }
        frame_count ++;
    }
	time_stamp_t base_time = time_point_cast<microseconds>(steady_clock::now());
	ts.calbr_basetime(base_time);
	ts.build_timeline("YoloV5");
	ts.show_summary("YoloV5 Demo");
	ts.clear();
}
