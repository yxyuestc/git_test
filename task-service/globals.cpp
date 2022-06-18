#include <atomic>
#include <unordered_map>
#include <string>
#include <functional>

#include "SyncQueue.hpp"
#include "dto/AlgoDto.hpp"

#include "../algorithms/algos.hpp"

namespace task_service {

/**
 * @brief 订阅标志, 用于开启订阅与取消订阅，多线程共用，所以这里使用原子变量
 * 
 */
std::atomic<bool> sub_flag;

/**
 * @brief 推送队列, 使用同步队列来存储对应线程的图片进行处理
 * 
 */
std::unordered_map<std::string, algorithms::SyncQueue<cv::Mat> *> threadsQueue;

/**
 * @brief 算法函数映射表，如果加了新的算法函数，在这里进行注册就行了
 * 
 */
std::unordered_map<std::string, std::function<void (oatpp::Object<dto::AlgoDto>)>> funcMap{
    {"areaInvade",      std::bind(algorithms::areaInvade,   std::placeholders::_1)},
    {"fire",            std::bind(algorithms::fire,         std::placeholders::_1)},
    {"helmet",          std::bind(algorithms::helmet,       std::placeholders::_1)},
    {"leave",           std::bind(algorithms::leave,        std::placeholders::_1)},
    {"personNumber",    std::bind(algorithms::personNumber, std::placeholders::_1)},
    {"sleep",           std::bind(algorithms::sleep1,       std::placeholders::_1)},
    {"smog",            std::bind(algorithms::smog,         std::placeholders::_1)},
    {"smoke",           std::bind(algorithms::smoke,        std::placeholders::_1)},
    {"xf",              std::bind(algorithms::xf,           std::placeholders::_1)}
};

}
