#ifndef Task_Service_Warning_Dto_hpp
#define Task_Service_Warning_Dto_hpp

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace task_service { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class WarningDto : public oatpp::DTO
{

    DTO_INIT(WarningDto, DTO)

    DTO_FIELD(Int64, id);                   // 报警信息id
    DTO_FIELD(String, alarm_code);          // 报警码
    DTO_FIELD(String, alarm_remark);        // 报警描述
    DTO_FIELD(String, alarm_time);          // 报警时间
    DTO_FIELD(String, alarm_video);         // 报警视频名
    DTO_FIELD(String, alarm_warning_img);   // 报警小图地址
    DTO_FIELD(String, alarm_img);           // 报警原图地址
};

#include OATPP_CODEGEN_END(DTO)

}}


#endif