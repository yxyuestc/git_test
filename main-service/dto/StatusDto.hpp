#ifndef Main_Service_Status_Dto_hpp
#define Main_Service_Status_Dto_hpp

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

using namespace oatpp;

namespace main_service { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class StatusDto : public oatpp::DTO
{
    DTO_INIT(StatusDto, DTO)

    DTO_FIELD_INFO(status) {
        info->description = "Short status text";
    }
    DTO_FIELD(String, status);

    DTO_FIELD_INFO(code) {
        info->description = "Status code";
    }
    DTO_FIELD(Int32, code);

    DTO_FIELD_INFO(message) {
        info->description = "Verbose message";
    }
    DTO_FIELD(String, message);
};

#include OATPP_CODEGEN_END(DTO)

}}

#endif