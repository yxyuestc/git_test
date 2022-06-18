#ifndef Main_Service_Runner_hpp
#define Main_Service_Runner_hpp

#include "oatpp/web/server/api/ApiController.hpp"

#include <list>
#include <thread>

namespace main_service {

class Runner
{
public:
    Runner();

    void run(std::list<std::thread> &acceptingThread);
};

}

#endif