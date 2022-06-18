#ifndef Algorithms_Algos_hpp
#define Algorithms_Algos_hpp

#include "../task-service/dto/AlgoDto.hpp"

#include <oatpp/core/Types.hpp>

namespace algorithms {

void areaInvade(oatpp::Object<::task_service::dto::AlgoDto> json_data);
void fire(oatpp::Object<::task_service::dto::AlgoDto> json_data);
void helmet(oatpp::Object<::task_service::dto::AlgoDto> json_data);
void leave(oatpp::Object<::task_service::dto::AlgoDto> json_data);
void personNumber(oatpp::Object<::task_service::dto::AlgoDto> json_data);
void sleep1(oatpp::Object<::task_service::dto::AlgoDto> json_data);
void smog(oatpp::Object<::task_service::dto::AlgoDto> json_data);
void smoke(oatpp::Object<::task_service::dto::AlgoDto> json_data);
void xf(oatpp::Object<::task_service::dto::AlgoDto> json_data);


}

#endif