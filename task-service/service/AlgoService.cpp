#include "AlgoService.hpp"
#include "../dto/StatusDto.hpp"
#include "../dto/AlgoDto.hpp"

#include "../../utils/Utils.hpp"

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>

#include <sstream>

namespace task_service { namespace service {

oatpp::Object<dto::StatusDto> AlgoService::insertAlgo(const Int64 &algoId)
{
    auto algoResult = m_algoDb->selectById(algoId);
    OATPP_ASSERT_HTTP(algoResult->isSuccess(), Status::CODE_500, algoResult->getErrorMessage());

    // 修改缓存
    std::stringstream cache = std::stringstream(*db::TaskDb::m_currentProcesstaskDto->algorithms);
    cache << "," << algoId;
    db::TaskDb::m_currentProcesstaskDto->algorithms = oatpp::String(cache.str());

    db::TaskDb::m_currentProcesstaskDto->update_time = oatpp::String(::utils::getNow());

    auto taskResult = m_taskDb->updateAlgo(
        db::TaskDb::m_currentProcesstaskDto->algorithms, 
        db::TaskDb::m_currentProcesstaskDto->update_time,
        db::TaskDb::m_currentProcesstaskDto->id);

    // 新开一个线程处理算法任务 TODO

    auto status = dto::StatusDto::createShared();
    status->status = "OK";
    status->code = 200;
    status->message =  "algorithm was successfully insert";
    return status;
}

oatpp::Object<dto::AlgoDto> AlgoService::selectById(const Int64 &algoId)
{
    auto dbResult = m_algoDb->selectById(algoId);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    oatpp::Object<dto::AlgoDto> result = dbResult->fetch<oatpp::Object<dto::AlgoDto>>();
    return result;
}

}}