#ifndef Task_Service_Task_Dto_hpp
#define Task_Service_Task_Dto_hpp

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace task_service { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class TaskDto : public oatpp::DTO
{
    DTO_INIT(TaskDto, DTO)

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
    DTO_FIELD(String, rtsp_address);
    DTO_FIELD(String, algorithms);
    DTO_FIELD(String, ip);
    DTO_FIELD(Int16, port);
    DTO_FIELD(String, current_time);
    DTO_FIELD(String, update_time);
};

#include OATPP_CODEGEN_END(DTO)

}}

#endif