#ifndef Task_Service_Algo_Dto_hpp
#define Task_Service_Algo_Dto_hpp

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

namespace task_service { namespace dto {

#include OATPP_CODEGEN_BEGIN(DTO)

class AlgoDto : public oatpp::DTO
{
    DTO_INIT(AlgoDto, DTO)

    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
    DTO_FIELD(Float64, conf);
    DTO_FIELD(Float64, obj);
    DTO_FIELD(Float64, iou);
    DTO_FIELD(Int32, dev_id);
    DTO_FIELD(String, pos_list);
    DTO_FIELD(Float64, var);
    DTO_FIELD(Int32, yd_time);
    DTO_FIELD(Int32, lg_time);
    DTO_FIELD(Int32, jg_time);
    DTO_FIELD(Int32, cz_time);
    DTO_FIELD(Int32, person_number);
};

#include OATPP_CODEGEN_END(DTO)

}} // namespace main_service 


#endif