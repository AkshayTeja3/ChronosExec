#ifndef THREAD_POOL_TPP
#define THREAD_POOL_TPP

namespace aee {

template<typename F, typename... Args>
auto ThreadPool::submit(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> 
{
    using return_type = typename std::result_of<F(Args...)>::type;
    
    // Create a packaged task (wraps function to return future)
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    // Get future before moving task to queue
    std::future<return_type> result = task->get_future();
    
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        tasks_.emplace([task]() { (*task)(); });
    }
    
    condition_.notify_one();
    return result;
}

} // namespace aee

#endif