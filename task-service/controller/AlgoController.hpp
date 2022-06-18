#ifndef Task_Service_Controller_AlgoController_hpp
#define Task_Service_Controller_AlgoController_hpp

#include "../Constants.hpp"
#include "../service/AlgoService.hpp"
#include "../dto/StatusDto.hpp"
#include "../dto/AlgoDto.hpp"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

using namespace oatpp::web::server::api;

namespace task_service { namespace  controller {

#include OATPP_CODEGEN_BEGIN(ApiController)

class AlgoController : public oatpp::web::server::api::ApiController
{
private:
    service::AlgoService m_algoService;

public:
    AlgoController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
    {}

    static std::shared_ptr<AlgoController> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper, Qualifier::SERVICE_TASK))
    {
        return std::make_shared<AlgoController>(objectMapper);
    }

public:
    ENDPOINT_INFO(addAlgo) 
    {
        info->summary = "add a new algorithm";

        info->addResponse<Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["algoId"].description = "algorithm id";
    }
    ENDPOINT("GET", "addAlgo/{algoId}", addAlgo,
            PATH(Int64, algoId)) 
    {
        return oatpp::web::server::api::ApiController::createDtoResponse(Status::CODE_200, m_algoService.insertAlgo(algoId));
    }

    ENDPOINT_INFO(queryById)
    {
        info->summary = "select a algorithm info by algorithm id";

        info->addResponse<Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["algoId"].description = "algorithm id";
    }
    ENDPOINT("GET", "queryById/{algoId}", queryById,
            PATH(Int64, algoId))
    {
        return oatpp::web::server::api::ApiController::createDtoResponse(Status::CODE_200, m_algoService.selectById(algoId));
    }
};

#include OATPP_CODEGEN_END(ApiController)

}}

#endif