#include "utils.hpp"

#include <chrono>

namespace utils {

std::string getNow()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    struct tm *tmNow = localtime(&tt);
    char timeNow[30] = {0};

    sprintf(timeNow, "%d-%02d-%02d %02d:%02d:%02d", (int)tmNow->tm_year + 1900, 
            (int)tmNow->tm_mon + 1, (int)tmNow->tm_mday, (int)tmNow->tm_hour,
            (int)tmNow->tm_min, (int)tmNow->tm_sec);

    return std::string(timeNow);
}

}