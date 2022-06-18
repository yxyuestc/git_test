#include "WarningService.hpp"
#include "../dto/StatusDto.hpp"
#include "../dto/WarningDto.hpp"
#include "../dto/PageDto.hpp"
#include "../globals.hpp"

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>

namespace task_service { namespace service {

oatpp::Object<dto::StatusDto> WarningService::subscribeWarn()
{
    sub_flag.store(true);
}

oatpp::Object<dto::StatusDto> WarningService::unsubscribeWarn()
{
    sub_flag.store(false);
}

oatpp::Object<dto::WarningDto> WarningService::getWarningById(Int64 warnId)
{
    auto dbResult = m_warningDb->selectById(warnId);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "History info not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<dto::WarningDto>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");
    return result[0];
}

oatpp::Object<dto::WarningsPageDto> WarningService::getWarningByPage(UInt32 offset, UInt32 limit)
{
    oatpp::UInt32 countToFetch = limit;

    if(limit > 10) {
        countToFetch = 10;
    }

    auto dbResult = m_warningDb->selectAllHistory(offset, countToFetch);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto items = dbResult->fetch<oatpp::Vector<oatpp::Object<dto::WarningDto>>>();

    auto page = dto::PageDto<oatpp::Object<dto::WarningDto>>::createShared();
    page->offset = offset;
    page->limit = countToFetch;
    page->count = items->size();
    page->items = items;

    return page;
}

}}