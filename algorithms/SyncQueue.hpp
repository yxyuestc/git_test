#ifndef Algorithm_SyncQueue_hpp
#define Algorithm_SyncQueue_hpp

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace algorithms {

template <typename T>
class SyncQueue
{
public:
    SyncQueue(): m_maxSize(50) {}
    SyncQueue(unsigned maxSize) : m_maxSize(maxSize) {}

    void put(std::shared_ptr<T> x)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        // 判断任务队列是不已经满了
        while (m_queue.size() == m_maxSize)
        {
            // 阻塞线程
            m_notFull.wait(locker);
            // std::shared_ptr<T> p = m_queue.front();
            m_queue.pop();
        }
        // 将任务放入到任务队列中
        m_queue.push(x);
        // 通知消费者去消费
        m_notEmpty.notify_one();
    }

    shared_ptr<T> take()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        while (m_queue.empty())
        {
            m_notEmpty.wait(locker);
        }
        // 从任务队列中取出任务(消费)
        std::shared_ptr<T> img = m_queue.front();
        m_queue.pop();
        // 通知生产者去生产
        m_notFull.notify_one();
        return img;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }

    bool full()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size() == m_maxSize;
    }

    unsigned size()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

private:
    std::queue<std::shared_ptr<T>> m_queue; // 存储队列数据
    std::mutex m_mutex;                     // 互斥锁
    std::condition_variable m_notEmpty;     // 不为空的条件变量
    std::condition_variable m_notFull;      // 没有满的条件变量
    unsigned m_maxSize;                     // 任务队列的最大任务个数
};

}

#endif