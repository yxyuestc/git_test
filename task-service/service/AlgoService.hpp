#ifndef Task_Service_Service_Algo_hpp
#define Task_Service_Service_Algo_hpp

#include "../Constants.hpp"

#include "../db/AlgoDb.hpp"
#include "../db/TaskDb.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/client/ApiClient.hpp>

namespace task_service { namespace service {

class AlgoService
{
private:
    typedef oatpp::web::protocol::http::Status Status;
private:
    OATPP_COMPONENT(std::shared_ptr<db::AlgoDb>, m_algoDb, Qualifier::SERVICE_TASK);
    OATPP_COMPONENT(std::shared_ptr<db::TaskDb>, m_taskDb, Qualifier::SERVICE_TASK);

public:
    oatpp::Object<dto::StatusDto> insertAlgo(const Int64 &algoId); 
    oatpp::Object<dto::AlgoDto> selectById(const Int64 &algoId);
};

}} // namespace task_service


#endif