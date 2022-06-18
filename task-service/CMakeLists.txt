cmake_minimum_required(VERSION 3.1)

set(project_name task_service)

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        Constants.hpp
        AppComponent.hpp
        SwaggerComponent.hpp
        controller/TaskController.hpp
        dto/StatusDto.hpp
        Runner.cpp
        Runner.hpp
        )

## include directories

target_include_directories(${project_name}-lib PUBLIC .)

## link libs

find_package(oatpp          1.3.0 REQUIRED)
find_package(oatpp-swagger  1.3.0 REQUIRED)
find_package(oatpp-sqlite   1.3.0 REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-sqlite
)

## define path to swagger-ui res folder

add_definitions(
    -DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res"
    
    -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/build/db.sqlite"

    -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"
)

set(SWAGGER_ROOT_PATH "/swagger" CACHE STRING "Default root path to the Swagger")
set(SWAGGER_UI_PATH "/ui" CACHE STRING "Default path suffix to the Swagger UI")
add_compile_definitions(
    SWAGGER_ROOT_PATH="${SWAGGER_ROOT_PATH}"
    SWAGGER_UI_PATH="${SWAGGER_UI_PATH}"
)
if(CMAKE_SYSTEM_NAME MATCHES Linux)
    find_package(Threads REQUIRED)
    target_link_libraries(${project_name}-lib INTERFACE Threads::Threads ${CMAKE_DL_LIBS})
endif()

## add executables

add_executable(${project_name}-exe App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)