#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>

namespace tcp_file_transfer {

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    // Delete copy constructor and assignment operator
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Enqueue a task
    template<class F, class... Args>
    auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    // Stop the thread pool
    void Stop();

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
};

// Implementation of the Enqueue method
template<class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex_);

        // Don't allow enqueueing after stopping the pool
        if (stop_) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks_.emplace([task]() { (*task)(); });
    }

    condition_.notify_one();
    return res;
}

} // namespace tcp_file_transfer

#endif // THREAD_POOL_H
