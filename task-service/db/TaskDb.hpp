#ifndef Task_Service_Db_Task_hpp
#define Task_Service_Db_Task_hpp

#include "../dto/TaskDto.hpp"

#include <oatpp-sqlite/orm.hpp>

using namespace oatpp::orm;

namespace task_service { namespace db {

#include OATPP_CODEGEN_BEGIN(DbClient)

class TaskDb : public oatpp::orm::DbClient
{
public:
    static oatpp::Object<dto::TaskDto> m_currentProcesstaskDto;

public:
    TaskDb(const std::shared_ptr<oatpp::orm::Executor> &executor) : oatpp::orm::DbClient(executor)
    {}

    QUERY(selectTaskByHostPort,
        "SELECT * FROM task WHERE (host = host AND port = port);",
        PARAM(String, host),
        PARAM(Int16, port))

    QUERY(updateAlgo,
        "UPDATE task SET"
        "algorithms = :insert_algorithm"  
        "update_time = :update_time"
        "WHERE id = :id;",
        PARAM(String, insert_algorithm),
        PARAM(String, update_time),
        PARAM(Int64, id))
};

#include OATPP_CODEGEN_END(DbClient)

}}

#endif