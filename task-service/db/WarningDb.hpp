#ifndef Task_Service_Db_Warning_hpp
#define Task_Service_Db_Warning_hpp

#include "../dto/WarningDto.hpp"
#include "../dto/PageDto.hpp"

#include <oatpp-sqlite/orm.hpp>

using namespace oatpp::orm;

namespace task_service { namespace db {

#include OATPP_CODEGEN_BEGIN(DbClient)

class WarningDb : public oatpp::orm::DbClient
{
public:
    WarningDb(const std::shared_ptr<oatpp::orm::Executor> &executor) : oatpp::orm::DbClient(executor)
    {}

    QUERY(selectById,
        "SELECT * FROM warning WHERE id = :warnId",
        PARAM(Int64, warnId))

    QUERY(selectAllHistory, 
        "SELECT * FROM warning LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit))
};

#include OATPP_CODEGEN_END(DbClient)

}} // namespace task_service


#endif