#ifndef Task_Service_TaskController_hpp
#define Task_Service_TaskController_hpp

#include "../Constants.hpp"
// #include "task-service/db/Database.hpp"
#include "../dto/StatusDto.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

namespace task_service { namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- codegen Begin

class TaskController : public oatpp::web::server::api::ApiController {
private:

  /**
   *  Inject Database component
   */
  //OATPP_COMPONENT(std::shared_ptr<db::Database>, m_database, Qualifiers::SERVICE_BOOK);

public:

  TaskController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper, Qualifier::SERVICE_TASK) /* Inject object mapper */)
    : oatpp::web::server::api::ApiController(objectMapper) {}

public:

  ENDPOINT_INFO(getTask) {
    info->summary = "Get a tast";
    //info->addConsumes<Object<dto::BookDto>>("application/json");
    //info->addResponse<Object<dto::BookDto>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "/query", getTask) {
    auto status = dto::StatusDto::createShared();
    status->status = "OK";
    status->code = 200;
    status->message =  "query task was successfully";
    return createDtoResponse(Status::CODE_200,status);
  }


 
};

#include OATPP_CODEGEN_END(ApiController) //<-- codegen End

}}

#endif /* example_book_BookController_hpp */
