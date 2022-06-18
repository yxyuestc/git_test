#include "ServerComponent.hpp"
#include "Runner.hpp"

#include <oatpp/network/Server.hpp>

/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
void run()
{
    // 创建组件环境
    main_service::ServerComponent components( {"0.0.0.0", 8000}, {"0.0.0.0", 8001} );

    // run
    std::list<std::thread> acceptingThreads;
    main_service::Runner runner;

    runner.run(acceptingThreads);

    for(auto &thread : acceptingThreads) thread.join();

}

int main(int argc, char const *argv[])
{
    oatpp::base::Environment::init();

    run();

    oatpp::base::Environment::destroy();
    return 0;
}
