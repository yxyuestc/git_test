#ifndef Task_Service_Page_Dto_hpp
#define Task_Service_Page_Dto_hpp

#include "controller/WarningController.hpp"

namespace task_service { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

template<class T>
class PageDto : public oatpp::DTO {

  DTO_INIT(PageDto, DTO)

  DTO_FIELD(UInt32, offset);
  DTO_FIELD(UInt32, limit);
  DTO_FIELD(UInt32, count);
  DTO_FIELD(Vector<T>, items);

};

class WarningsPageDto : public PageDto<oatpp::Object<WarningDto>> {

  DTO_INIT(WarningsPageDto, PageDto<oatpp::Object<WarningDto>>)

};

#include OATPP_CODEGEN_END(DTO)

}}

#endif