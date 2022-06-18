#ifndef Task_Service_Service_Warning_hpp
#define Task_Service_Service_Warning_hpp

#include "../Constants.hpp"

#include "../dto/StatusDto.hpp"
#include "../dto/WarningDto.hpp"
#include "../dto/PageDto.hpp"

#include "../db/WarningDb.hpp"

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/client/ApiClient.hpp>

namespace task_service { namespace service {

class WarningService
{
private:
    typedef oatpp::web::protocol::http::Status Status;

private:
    OATPP_COMPONENT(std::shared_ptr<db::WarningDb>, m_warningDb, Qualifier::SERVICE_TASK);

public:
    oatpp::Object<dto::StatusDto> subscribeWarn();         // 订阅历史报警信息
    oatpp::Object<dto::StatusDto> unsubscribeWarn();
    oatpp::Object<dto::WarningDto> getWarningById(Int64 warnId);
    oatpp::Object<dto::WarningsPageDto> getWarningByPage(UInt32 offset, UInt32 limit);
};

}}

#endif