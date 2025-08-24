#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace crawler {

/**
 * @brief A simple thread pool implementation
 * 
 * This class manages a pool of worker threads that can execute tasks.
 */
class ThreadPool {
public:
    /**
     * @brief Construct a new Thread Pool object
     * 
     * @param num_threads Number of worker threads to create
     */
    explicit ThreadPool(size_t num_threads);

    /**
     * @brief Destroy the Thread Pool object
     */
    ~ThreadPool();

    // Delete copy constructor and assignment operator
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    /**
     * @brief Enqueue a task to be executed by the thread pool
     * 
     * @param task The task to execute
     */
    void Enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> stop_;
};

} // namespace crawler

#endif // THREAD_POOL_H