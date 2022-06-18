#ifndef Task_Service_Threadpool_Threadpool_hpp
#define Task_Service_Threadpool_Threadpool_hpp

#include <vector>
#include <queue>
#include <memory.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace task_service { namespace threadPool {

class ThreadPool
{
public:
    ThreadPool(size_t);

    template<typename F, typename ...Args>
    std::future<typename std::result_of<F(Args ...)>::type> enqueue(const std::string &threadName, F &&f, Args &&...args);

    ~ThreadPool();

private:
    std::vector<std::thread> m_workers;

    std::queue<std::pair<std::string, std::function<void ()>>> m_tasks;

    std::mutex m_queue_mutex;

    std::condition_variable m_condition;

    bool m_stop;
};

inline ThreadPool::ThreadPool(size_t threads) : m_stop(false)
{
    for(size_t i = 0;i < threads;i++){
        m_workers.emplace_back( [this] {
            while(true)
            {
                std::function<void ()> task;

                {
                    std::unique_lock<std::mutex> lock(m_queue_mutex);
                    m_condition.wait(lock, [this] {
                        return m_stop || !m_tasks.empty();
                    });
                    if(m_stop && m_tasks.empty()) return;
                    task = std::move(m_tasks.front().second);
                    m_tasks.pop();
                }

                task();
            }
        });
    }
}

// std::result_of 用于推导模板返回值的类型，在这个地方用于推导线程返回结果类型
template <typename F, typename ...Args>
inline std::future<typename std::result_of<F(Args...)>::type> ThreadPool::enqueue(const std::string &threadName, F &&f, Args &&...args)
{
    using return_type = typename std::result_of<F(Args ...)>::type;

    // std::packaged_task用于将线程返回结果返回给std::future
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        // 绑定线程函数
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    // 获取线程返回结果, 异步传输线程返回
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);

        if(m_stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        
        m_tasks.emplace(std::pair<std::string, std::function<void ()>>(threadName, [task] () { (*task)(); }));  // 将线程加入线程池
    }
    m_condition.notify_one();
    return res;
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_stop = true;
    }
    m_condition.notify_all();

    for (std::thread &worker : m_workers)
        worker.join();
}

}}

#endif