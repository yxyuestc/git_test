
#ifndef task_service_Runner_hpp
#define task_service_Runner_hpp

#include "oatpp/web/server/api/ApiController.hpp"

#include <list>
#include <thread>

namespace task_service {

class Runner {
public:

  Runner();

  Runner(const char *ip_addr, int port);

  void run(std::list<std::thread>& acceptingThreads);

};

}

#endif // example_book_Runner_hpp
