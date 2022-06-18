#ifndef Main_Service_DatabaseComponent_hpp
#define Main_Service_DatabaseComponent_hpp

#include "Constants.hpp"
#include "db/TaskDb.hpp"

#include <oatpp/core/macro/component.hpp>

namespace main_service {

class DatabaseComponent
{
public:
    // 创建数据库连接组件
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider)(Qualifier::SERVICE_MAIN, [] {
        // cmake编译文件中设置数据库文件
        auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(DATABASE_FILE);

        return oatpp::sqlite::ConnectionPool::createShared(connectionProvider, 10, std::chrono::seconds(5));
    }());


    OATPP_CREATE_COMPONENT(std::shared_ptr<db::TaskDb>, taskDb)(Qualifier::SERVICE_MAIN, [] {
        
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, connectionProvider, Qualifier::SERVICE_MAIN);

        auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);

        return std::make_shared<db::TaskDb>(executor);
    }());
};

}
#endif