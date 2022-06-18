#ifndef Main_Service_Controller_TaskController_hpp
#define Main_Service_Controller_TaskController_hpp


#include "../Constants.hpp"
#include "../service/TaskService.hpp"
#include "../dto/StatusDto.hpp"
#include "../dto/TaskDto.hpp"
#include <typeinfo>
#include <iostream>

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace main_service { namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController) //<--- codegen begin

class TaskController : public oatpp::web::server::api::ApiController {
private:

  OATPP_COMPONENT(std::shared_ptr<service::TaskService>, TaskService, Qualifier::SERVICE_MAIN);

public:

  TaskController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper, Qualifier::SERVICE_MAIN) /* Inject object mapper */)
    : oatpp::web::server::api::ApiController(objectMapper) {}

public:

    ENDPOINT_INFO(getTask) {
        info->summary = "get task";
        // info->addResponse<List<dto::TaskDto>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/query", getTask) {
        auto response = TaskService->getTask();
        auto status = response->readBodyToDto<oatpp::Object<dto::StatusDto>>(getDefaultObjectMapper().get());
        std::cout<<"hello"<<std::endl;
        std::cout<<typeid(status).name()<<std::endl;
        return createDtoResponse(Status::CODE_200, status);
    }

};

#include OATPP_CODEGEN_END(ApiController) //<--- codegen end

}}

#endif /* example_facade_TaskController_hpp */
