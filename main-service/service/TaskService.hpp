#ifndef Main_Service_Service_Task_hpp
#define Main_Service_Service_Task_hpp

#include "../dto/TaskDto.hpp"
#include "../dto/StatusDto.hpp"
#include "../db/TaskDb.hpp"
#include "../Constants.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/client/ApiClient.hpp>
#include "oatpp/core/macro/codegen.hpp"

namespace main_service { namespace service {

#include OATPP_CODEGEN_BEGIN(ApiClient)

class TaskService : public oatpp::web::client::ApiClient
{
private:
    typedef oatpp::web::protocol::http::Status Status;
private:

public:

    API_CLIENT_INIT(TaskService)

    API_CALL("GET", "/query", getTask)

    //oatpp::Object<dto::StatusDto> insertTask(const oatpp::Object<dto::TaskDto> &dto);
    //oatpp::Object<dto::StatusDto> updateTask(const oatpp::Object<dto::TaskDto> &dto);
};

#include OATPP_CODEGEN_END(ApiClient)

}}

#endif