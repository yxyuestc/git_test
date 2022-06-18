#include "yolov5.hpp"
#include "opencv2/opencv.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <vector>


using namespace std;
using namespace cv;

//判断两条线是否相交
bool intersection(const vector<cv::Point> &line1, const vector<cv::Point> &line2)
{
    assert(line1.size() == line2.size());
    assert(line1.size() == 2);

    cv::Point point1_11, point1_12, point1_21, point1_22;
    point1_11 = line2[0] - line1[0];
    point1_12 = line2[1] - line1[0];

    point1_21 = line2[0]-line1[1];
    point1_22 = line2[1]-line1[1];

    //point1_11.cross(point1_12)*point1_21.cross(point1_22)<0;
    cv::Point point2_11,point2_12,point2_21,point2_22;

    point2_11 = line1[0] - line2[0];
    point2_12 = line1[1] - line2[0];

    point2_21 = line1[0] - line2[1];
    point2_22 = line1[1] - line2[1];

    //point2_11.cross(point2_12)*point2_21.cross(point2_22)<0;
    return ( point1_11.cross(point1_12)*point1_21.cross(point1_22)<0 &&
             point2_11.cross(point2_12)*point2_21.cross(point2_22)<0 );
}

//判断两个矩形框是否重叠
bool isOverlap(const YoloV5Box &bbox, const std::vector<cv::Point> &points)
{
    // rect1中x的范围[rect1_x1, rect1_x2]，y的范围[rect1_y1, rect1_y2]
    int rect1_x1 = bbox.x;
    int rect1_x2 = bbox.x + bbox.width;
    int rect1_y1 = bbox.y;
    int rect1_y2 = bbox.y + bbox.height;
    
    // rect2中x的范围[rect2_x1, rect2_x2]，y的范围[rect2_y1, rect2_y2]
    int rect2_x1 = points[0].x;
    int rect2_x2 = points[2].x;
    int rect2_y1 = points[0].y;
    int rect2_y2 = points[2].y;
 
    if(rect1_x1 > rect2_x2 || rect1_x2 < rect2_x1 || rect1_y1 > rect2_y2 || rect1_y2 < rect2_y1) {
        return false;
    }
    else {
        return true;
    }
}

//判断点是否位于不规则矩形内
bool judgeIrregular(const YoloV5Box &bbox, const std::vector<cv::Point> &points)
{
    int count = 0;
    int pp[4][2] = {
        {bbox.x, bbox.y},
        {bbox.x + bbox.width, bbox.y},
        {bbox.x + bbox.width, bbox.y + bbox.height},
        {bbox.x, bbox.y + bbox.height}
    };
    for(int i = 0;i < 4;i++)
    {
        cv::Point line11 = cv::Point(pp[i][0], pp[i][1]);
        cv::Point line12 = cv::Point(0, pp[i][1]);
        std::vector<cv::Point> line1{line11, line12};
        for(size_t j = 0;j < points.size();j++)
        {
            cv::Point line21 = cv::Point(points[j].x, points[j].y);
            cv::Point line22 = j != 0 ? cv::Point(points[j - 1].x, points[j - 1].y) :
                cv::Point(points[points.size() - 1].x, points[points.size() - 1].y);
            std::vector<cv::Point> line2{line21, line22};
            if(intersection(line1, line2)) count++;
        }

        if(count % 2) return false;
    }
    return true;
}

//判断矩形和不规则图像是否重叠
bool judgetoregular(const YoloV5Box &bbox, const std::vector<cv::Point> &points){
    int pp[4][2] = {
        {bbox.x, bbox.y},
        {bbox.x + bbox.width, bbox.y},
        {bbox.x + bbox.width, bbox.y + bbox.height},
        {bbox.x, bbox.y + bbox.height}
    };

    for(int i = 0;i < 4;i++)
    {
        cv::Point line11 = cv::Point(pp[i][0], pp[i][1]);
        cv::Point line12 = i != 0 ? cv::Point(pp[i-1][0], pp[i-1][1]) :
                                    cv::Point(pp[3][0], pp[3][1]);
        std::vector<cv::Point> line1{line11, line12};

        for(size_t j = 0;j < points.size();j++)
            {
                cv::Point line21 = cv::Point(points[j].x, points[j].y);
                cv::Point line22 = j != 0 ? cv::Point(points[j - 1].x, points[j - 1].y) :
                    cv::Point(points[points.size() - 1].x, points[points.size() - 1].y);
                std::vector<cv::Point> line2{line21, line22};
                if(intersection(line1, line2)) 
                    return true;
            }
    }
    return false;
}


//判断行人框最下面的线的两个端点是否在不规则形状内部
// true证明不相交  false证明相交或者在内部
bool leave_judge(const YoloV5Box &bbox, const std::vector<cv::Point> &points)
{
    int count = 0;
    int pp[2][2] = {
        {bbox.x + bbox.width, bbox.y + bbox.height},
        {bbox.x, bbox.y + bbox.height}
    };

    //判断行人框最下面的线的两个端点是否都在不规则形状内部
    for(int i = 0;i < 2;i++)
    {
        cv::Point line11 = cv::Point(pp[i][0], pp[i][1]);
        cv::Point line12 = cv::Point(0, pp[i][1]);
        std::vector<cv::Point> line1{line11, line12};
        for(size_t j = 0;j < points.size();j++)
        {
            cv::Point line21 = cv::Point(points[j].x, points[j].y);
            cv::Point line22 = j != 0 ? cv::Point(points[j - 1].x, points[j - 1].y) :
                cv::Point(points[points.size() - 1].x, points[points.size() - 1].y);
            std::vector<cv::Point> line2{line21, line22};
            if(intersection(line1, line2)) count++;
        }

        if(count % 2 ) return false;  //奇数返回false  在内部
    }

    //判断行人框的下边缘线是否与检测区域的边有交点，存在交点，就是在检测区域内
    //存在一种漏判的情况，两个点都在不规则形状内部
    cv::Point line11 = cv::Point(pp[0][0], pp[0][1]);
    cv::Point line12 = cv::Point(pp[1][0], pp[1][1]);
    std::vector<cv::Point> line1{line11, line12};
    for(size_t j = 0;j < points.size();j++)
        {
            cv::Point line21 = cv::Point(points[j].x, points[j].y);
            cv::Point line22 = j != 0 ? cv::Point(points[j - 1].x, points[j - 1].y) :
                cv::Point(points[points.size() - 1].x, points[points.size() - 1].y);
            std::vector<cv::Point> line2{line21, line22};
            if(intersection(line1, line2)) 
                return false;
        }
    return true;
}