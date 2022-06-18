#include <iostream>
#include <vector>
#include <chrono>

#include "ThreadPool.hpp"

int main()
{
    
    ::task_service::threadPool::ThreadPool pool(12);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 12; ++i) {
        results.emplace_back(
            pool.enqueue("thread" + i, [i] (int j) {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::cout << "world " << i << std::endl;
                return i * i * j;
            }, 10)
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
    
    return 0;
}
