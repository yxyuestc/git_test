#ifndef Task_Service_Db_Algo_hpp
#define Task_Service_Db_Algo_hpp

#include "../dto/AlgoDto.hpp"

#include <oatpp-sqlite/orm.hpp>
#include <unordered_map>

using namespace oatpp::orm;

namespace task_service { namespace db {

#include OATPP_CODEGEN_BEGIN(DbClient)

class AlgoDb : public oatpp::orm::DbClient
{
public:
    static std::unordered_map<Int64, oatpp::Object<dto::AlgoDto>> m_AlgorithmConfiguration;

public:
    AlgoDb(const std::shared_ptr<oatpp::orm::Executor> &executor) : oatpp::orm::DbClient(executor)
    {}

     QUERY(selectById,
        "SELECT * FROM algo WHERE id = :algoId;",
        PARAM(Int64, algoId))

    QUERY(selectAll,
        "SELECT * from algo;")
};

#include OATPP_CODEGEN_END(DbClient)

}} // namespace task_service


#endif