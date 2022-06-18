
#include <cstring>

#include "ServerComponent.hpp"
#include "Runner.hpp"

void run(const char *ip_addr, int port)
{
    // 创建组件环境
    task_service::ServerComponent components( {oatpp::String(ip_addr), port} );

    // run
    std::list<std::thread> acceptingThreads;
    task_service::Runner runner(ip_addr, port);

    runner.run(acceptingThreads);

    for(auto &thread : acceptingThreads) thread.join();
}

int main(int argc, const char *argv[])
{
    oatpp::base::Environment::init();
    
    if(argc > 1)
    {
        OATPP_LOGI("start a micro service, and task ip is %s and port is %d", argv[1], argv[2]);
        run(argv[1], atoi(argv[2]));
    }
    else
    {
        OATPP_LOGE("%s", "do not start a task, it's a illegal operator");
    }

    oatpp::base::Environment::destroy();
    return 0;
}
