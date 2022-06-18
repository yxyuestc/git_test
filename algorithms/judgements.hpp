#ifndef JUDGEMENTS_H
#define JUDGEMENTS_H

#include "yolov5.hpp"
#include "opencv2/opencv.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <vector>

using namespace cv;
using namespace std;


bool intersection(const vector<cv::Point> &line1, const vector<cv::Point> &line2);
bool isOverlap (const YoloV5Box &bbox, const std::vector<cv::Point> &points);
bool judgeIrregular(const YoloV5Box &bbox, const std::vector<cv::Point> &points);
bool judgetoregular(const YoloV5Box &bbox, const std::vector<cv::Point> &points);
bool leave_judge(const YoloV5Box &bbox, const std::vector<cv::Point> &points);

#endif