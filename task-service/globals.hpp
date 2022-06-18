#ifndef Task_Service_Globals_hpp
#define Task_Service_Globals_hpp

#include <atomic>
#include <unordered_map>
#include <string>
#include <functional>

#include "dto/AlgoDto.hpp"

#include "../algorithms/algos.hpp"
#include "SyncQueue.hpp"

namespace task_service {

extern std::atomic<bool> sub_flag;
extern std::unordered_map<std::string, algorithms::SyncQueue<cv::Mat> *> threadsQueue;
extern std::unordered_map<std::string, std::function<void (oatpp::Object<dto::AlgoDto>)>> funcMap;

}

#endif