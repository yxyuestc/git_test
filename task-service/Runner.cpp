#include "Runner.hpp"
#include "ServerComponent.hpp"
#include "controller/AlgoController.hpp"
#include "../utils/parseMulti.hpp"
#include "globals.hpp"

#include "controller/TaskController.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"

namespace task_service {

Runner::Runner(const char *ip_addr, int port)
{
    // 缓存当前进程算法查询结果，避免重新查询降低性能
    OATPP_COMPONENT(std::shared_ptr<db::TaskDb>, taskDb, Qualifier::SERVICE_TASK);
    auto dbResult = taskDb->selectTaskByHostPort(oatpp::String(ip_addr), (Int16)port);
    OATPP_ASSERT(dbResult->isSuccess());
    db::TaskDb::m_currentProcesstaskDto = dbResult->fetch<oatpp::Object<dto::TaskDto>>();

    // 缓存算法表中的配置查询结果，避免重新查询降低性能
    OATPP_COMPONENT(std::shared_ptr<db::AlgoDb>, algoDb, Qualifier::SERVICE_TASK);
    dbResult = algoDb->selectAll();
    OATPP_ASSERT(dbResult->isSuccess());
    auto res = dbResult->fetch<oatpp::Vector<oatpp::Object<dto::AlgoDto>>>();
    for(auto i = 0;i < res->size();i++)
        db::AlgoDb::m_AlgorithmConfiguration[res->at(i)->id] = res->at(i);

    // 线程池运行对应的线程
    OATPP_COMPONENT(std::shared_ptr<threadPool::ThreadPool>, aThreadPool, Qualifier::SERVICE_TASK);
    std::vector<int> parse_data;
    ::utils::parse(*db::TaskDb::m_currentProcesstaskDto->algorithms, parse_data);
    for(auto it = parse_data.begin(); it != parse_data.end(); ++it)
    {
        aThreadPool->enqueue(db::AlgoDb::m_AlgorithmConfiguration[*it]->name, 
                             funcMap[db::AlgoDb::m_AlgorithmConfiguration[*it]->name], 
                             db::AlgoDb::m_AlgorithmConfiguration[*it]);
    }
    
    // 注入controller
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router, Qualifier::SERVICE_TASK);

  oatpp::web::server::api::Endpoints docEndpoints;

  /* Add TaskController */
  docEndpoints.append(router->addController(std::make_shared<controller::TaskController>())->getEndpoints());

  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo, Qualifier::SERVICE_TASK);
  OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources, Qualifier::SERVICE_TASK);

  router->addController(oatpp::swagger::Controller::createShared(docEndpoints, documentInfo, resources));

}

void Runner::run(std::list<std::thread>& acceptingThreads) {

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router, Qualifier::SERVICE_TASK);

  /* Create connection handler */
  auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

  acceptingThreads.push_back(std::thread([router, connectionHandler]{

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider, Qualifier::SERVICE_TASK);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("book-service", "server is listening on port '%s'", connectionProvider->getProperty("port").getData());
    server.run();

  }));


}

}
