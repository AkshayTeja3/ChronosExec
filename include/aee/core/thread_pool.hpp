#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace aee {
    class ThreadPool {
public:
    // Constructor: takes number of threads
    explicit ThreadPool(size_t num_threads);
    
    // Destructor: joins all threads
    ~ThreadPool();
    
    // Submit a task (template function)
    template<typename F, typename... Args>
    auto submit(F&& f, Args&&...args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    
    // Wait for all pending tasks to complete
    void wait_for_all();
    
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_;
};

} // namespace aee
#include "thread_pool.tpp"
#endif
