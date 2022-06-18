#ifndef Task_Service_Controller_WarningController_hpp
#define Task_Service_Controller_WarningController_hpp

#include "../Constants.hpp"
#include "../service/WarningService.hpp"
#include "../dto/StatusDto.hpp"
#include "../dto/WarningDto.hpp"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

using namespace oatpp::web::server::api;

namespace task_service { namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController)

class WarningController : public oatpp::web::server::api::ApiController
{
private:

    service::WarningService m_warningService;

public:

    WarningController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
        : oatpp::web::server::api::ApiController(objectMapper)
    {}

    static std::shared_ptr<WarningController> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper, Qualifier::SERVICE_TASK))
    {
        return std::make_shared<WarningController>(objectMapper);
    }

public:

    ENDPOINT_INFO(subscribe)
    {
        info->summary = "subscribe warning info";

        info->addResponse<Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "/sub", subscribe)
    {
        return oatpp::web::server::api::ApiController::createDtoResponse(Status::CODE_200, m_warningService.subscribeWarn());
    }

    ENDPOINT_INFO(unsubscribe)
    {
        info->summary = "unsubscribe warning info";

        info->addResponse<Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "/unsub", unsubscribe)
    {
        return oatpp::web::server::api::ApiController::createDtoResponse(Status::CODE_200, m_warningService.unsubscribeWarn());
    }

    ENDPOINT_INFO(getWarnById)
    {
        info->summary = "get a warning info by warnId";

        info->addResponse<Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500, "application/json");

        info->pathParams["warnId"].description = "warning history Info Identifier";
    }
    ENDPOINT("GET", "warning/{warnId}", getWarnById, 
            PATH(Int64, warnId))
    {
        return createDtoResponse(Status::CODE_200, m_warningService.getWarningById(warnId));
    }

    ENDPOINT_INFO(getPageHistory)
    {
        info->summary = "get all warning info";

        info->addResponse<Object<dto::WarningsPageDto>>(Status::CODE_200, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "warning/{offset}/limit/{limit}", getPageHistory, 
            PATH(UInt32, offset),
            PATH(UInt32, limit))
    {
        return createDtoResponse(Status::CODE_200, m_warningService.getWarningByPage(offset, limit));
    }
};

#include OATPP_CODEGEN_END(ApiController)

}}


#endif