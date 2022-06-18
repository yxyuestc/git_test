#include "Runner.hpp"
#include "Constants.hpp"
#include "ServerComponent.hpp"
#include "controller/TaskController.hpp"

#include <oatpp-swagger/Controller.hpp>

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/Server.hpp>

namespace main_service {

Runner::Runner()
{
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router, Qualifier::SERVICE_MAIN);

    oatpp::web::server::api::Endpoints docEndpoints;

    docEndpoints.append(router->addController(std::make_shared<controller::TaskController>())->getEndpoints());

    OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo, Qualifier::SERVICE_MAIN);
    OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources, Qualifier::SERVICE_MAIN);

    router->addController(oatpp::swagger::Controller::createShared(docEndpoints, documentInfo, resources));    
}

void Runner::run(std::list<std::thread> &acceptingThreads)
{
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router, Qualifier::SERVICE_MAIN);

    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

    acceptingThreads.push_back(std::thread([router, connectionHandler] {

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider, Qualifier::SERVICE_MAIN);
        oatpp::network::Server server(connectionProvider, connectionHandler);
        OATPP_LOGI("service-main", "server is listening on port '%s'", connectionProvider->getProperty("port").getData());

        server.run();

        // 停止链接池
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider, Qualifier::SERVICE_MAIN);
        dbConnectionProvider->stop();
    }));
}

}