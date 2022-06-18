#ifndef Main_Service_Db_Task_hpp
#define Main_Service_Db_Task_hpp

#include "../dto/TaskDto.hpp"

#include <oatpp-sqlite/orm.hpp>

using namespace oatpp::orm;

namespace main_service { namespace db {

#include OATPP_CODEGEN_BEGIN(DbClient)

class TaskDb : public oatpp::orm::DbClient
{
public:
    TaskDb(const std::shared_ptr<oatpp::orm::Executor> &executor) : oatpp::orm::DbClient(executor)
    {
        oatpp::orm::SchemaMigration migration(executor);
        
        migration.addFile(1, "../../sql/project.sql");
        migration.migrate();

        auto version = executor->getSchemaVersion();
        OATPP_LOGD("TaskDb", "Migration - OK. Version=%lld.", version);
    }

    QUERY(insertTask,
        "INSERT INTO task "
        "(id, name, rtsp_address, algorithms, ip, port, current_time, update_time) "
        "VALUES (:task.id, :task.name, :task.rtsp_address, :task.algorithms, :task.ip, :task.port, :task.current_time, :task.update_time);",
        PARAM(oatpp::Object<dto::TaskDto>, task)) 

    QUERY(updateTask,
        "UPDATE task SET"
        "name = :newTask.name"
        "rtsp_address = :newTask.rtsp_address" 
        "algorithms = :newTask.algorithms" 
        "ip = :newTask.ip" 
        "port = :newTask.port" 
        "current_time = :newTask.current_time" 
        "update_time = :newTask.update_time"
        "WHERE id = :newTask.id",
        PARAM(oatpp::Object<dto::TaskDto>, newTask))
};

#include OATPP_CODEGEN_END(DbClient)

}}

#endif