#include <iostream>
#include <unordered_map>
#include <chrono>
#include <atomic>

#include "ThreadPool.hpp"

// 维护一个原子标志哈希表
std::unordered_map<std::string, std::atomic<bool>> flags;

// 通过原子操作将线程杀死，线程池中维护4个线程。该线程并不支持动态增加线程。
int main()
{
    ::task_service::threadPool::ThreadPool pool(4);
    std::vector< std::future<void> > results;
 
    for(int i = 0; i < 12; ++i) {
        results.emplace_back(
            pool.enqueue("thread" + i, [i] {
                while(true)
                {
                    if(flags["thread" + i].load()) break;
                    std::cout << "hello " << i << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << "world " << i << std::endl;
                }
            })
        );
        flags["thread" + i].store(false);
    }

    for(int i = 0;i < 4;i++)
    {
        // 通过对应原子标志将线程归还线程池
        flags["thread" + i].store(true);
        std::cout << "第" << i << "个线程被结束" << std::endl;
    }
}