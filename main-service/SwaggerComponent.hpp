#ifndef Main_Service_SwaggerComponent_hpp
#define Main_Service_SwaggerComponent_hpp

#include "Constants.hpp"

#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>
#include <oatpp/core/macro/component.hpp>

/**
 *  Swagger ui is served at
 *  http://host:port/swagger/ui
 */

namespace main_service {

class SwaggerComponent
{
public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)(Qualifier::SERVICE_MAIN, [] {

        oatpp::swagger::DocumentInfo::Builder builder;
    
        builder
        .setTitle("User entity service")
        .setDescription("CRUD API Example project with swagger docs")
        .setVersion("1.0")
        .setContactName("Ivan Ovsyanochka")
        .setContactUrl("https://oatpp.io/")
        
        .setLicenseName("Apache License, Version 2.0")
        .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")
        
        .addServer("http://0.0.0.0:8000", "server on localhost");
        
        return builder.build();
    
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResource)(Qualifier::SERVICE_MAIN, [] {
        return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
    }());
};

}

#endif