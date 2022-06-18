#ifndef Task_Service_ServerComponent_hpp
#define Task_Service_ServerComponent_hpp

#include "SwaggerComponent.hpp"
#include "DatabaseComponent.hpp"
#include "threadPool/ThreadPool.hpp"

#include "Constants.hpp"

#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/component.hpp>

namespace task_service {

struct HostPort
{
    oatpp::String host;
    v_uint16 port;
};

class ServerComponent
{
private:
    HostPort m_hostPort;

public:
    ServerComponent(const HostPort &hostPort) : m_hostPort(hostPort) {}

    SwaggerComponent swaggerComponent(HostPort &);

    // DatabaseComponent databaseComponent;

    // 初始化网络连接
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)(Qualifier::SERVICE_TASK, [&] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared( {m_hostPort.host, m_hostPort.port} );
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)(Qualifier::SERVICE_TASK, [] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)(Qualifier::SERVICE_TASK, [] {
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
        return objectMapper;
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<threadPool::ThreadPool>, algoThreadPool)(Qualifier::SERVICE_TASK, [] {
        return std::make_shared<threadPool::ThreadPool>(8);
    }());
};

}

#endif
