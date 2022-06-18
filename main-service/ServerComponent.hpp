#ifndef Main_Service_ServerComponent_hpp
#define Main_Service_ServerComponent_hpp

#include "DatabaseComponent.hpp"
#include "SwaggerComponent.hpp"
#include "ClientsComponent.hpp"
#include "Constants.hpp"
#include "service/TaskService.hpp"

#include <oatpp/web/server/HttpRouter.hpp>
#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/network/ConnectionPool.hpp"

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

#include <oatpp/core/macro/component.hpp>

namespace main_service {

struct HostPort
{
    oatpp::String host;
    v_uint16 port;
};

class ServerComponent
{
private:
    HostPort m_main;
    HostPort m_task;
public:

    ServerComponent(const HostPort &main_, const HostPort &task_) 
    : m_main(main_) 
    , m_task(task_)
    {}

    // 创建数据库链接组件
    DatabaseComponent databaseComponent;

    // 链接客户端列表
    ClientsComponent clientsComponent;

    // swagger 组件
    SwaggerComponent swaggerComponent;

    // 网络监听器
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)(Qualifier::SERVICE_MAIN, [this] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared({m_main.host, m_main.port, oatpp::network::Address::IP_4});
    }());

    // 路由组件
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)(Qualifier::SERVICE_MAIN, [] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    // 创建 ObjectMapper 组件以在 Contoller 的 API 中序列化/反序列化 DTO
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)(Qualifier::SERVICE_MAIN, [] {
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
        return objectMapper;
    }());

    // 创建 TaskService component
    OATPP_CREATE_COMPONENT(std::shared_ptr<service::TaskService>, taskService)(Qualifier::SERVICE_MAIN, [this] {

        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper, Qualifier::SERVICE_MAIN);

        std::shared_ptr<oatpp::network::ClientConnectionProvider> connectionProvider;

        connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({m_task.host, m_task.port});
        

        auto connectionPool = oatpp::network::ClientConnectionPool::createShared(connectionProvider, 10, std::chrono::seconds(5));

        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionPool);
        return service::TaskService::createShared(requestExecutor, objectMapper);

  }());

};

}

#endif