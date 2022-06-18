#ifndef Main_Service_ClientsComponent_hpp
#define Main_Service_ClientsComponent_hpp

#include "Constants.hpp"

#include <unordered_map>

#include <oatpp/core/macro/component.hpp>

namespace main_service {

struct TaskInfo
{
    pid_t m_processId;        // 进程号用于杀死进程
    oatpp::String m_taskName; // 任务名
    oatpp::String m_host;     // 任务的ip地址
    v_uint16    m_port;       // 端口号

    TaskInfo() = default;

    TaskInfo(const TaskInfo &taskInfo)
        : m_processId(taskInfo.m_processId)
        , m_taskName(taskInfo.m_taskName)
        , m_host(taskInfo.m_host)
        , m_port(taskInfo.m_port)
    {}

    TaskInfo(const pid_t &processId, const std::string &taskName, const std::string &host, const v_uint16 &port)
        : m_processId(processId)
        , m_taskName(taskName)
        , m_host(host)
        , m_port(port)
    {}
};

class ClientsComponent
{
private:
    std::unordered_map<oatpp::String, TaskInfo> m_clients;

public:

    OATPP_CREATE_COMPONENT(std::shared_ptr<ClientsComponent>, clients)(Qualifier::SERVICE_MAIN, [] {
        return nullptr;
    }());

    std::shared_ptr<std::unordered_map<oatpp::String, TaskInfo>> get_mapPoint() const 
    { return std::make_shared<std::unordered_map<oatpp::String, TaskInfo>>(m_clients); }

    void addTask(const TaskInfo taskInfo)
    {
        m_clients[taskInfo.m_taskName] = taskInfo;
    }

    void delTask(const std::string &taskName)
    {
        m_clients.erase(taskName);
    }
};

}

#endif