#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

namespace chat_room {

/**
 * @brief A simple thread pool implementation.
 * 
 * This class manages a pool of worker threads and a queue of tasks.
 * Tasks can be enqueued using the `Enqueue` method, which returns
 * a `std::future` for the result of the task.
 */
class ThreadPool {
public:
    /**
     * @brief Constructs a ThreadPool with a specified number of threads.
     * 
     * @param num_threads The number of worker threads to create.
     */
    explicit ThreadPool(size_t num_threads);

    /**
     * @brief Destructor. Stops all worker threads and waits for them to finish.
     */
    ~ThreadPool();

    /**
     * @brief Enqueues a task to be executed by a worker thread.
     * 
     * @tparam F The type of the task (usually a lambda or std::function).
     * @tparam Args The types of the arguments to pass to the task.
     * @param f The task to execute.
     * @param args The arguments to pass to the task.
     * @return A std::future representing the result of the task.
     */
    template<typename F, typename... Args>
    auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>;

    /**
     * @brief Gets the number of worker threads in the pool.
     * 
     * @return The number of worker threads.
     */
    size_t GetNumThreads() const { return threads_.size(); }

private:
    std::vector<std::thread> threads_;              ///< Vector of worker threads
    std::queue<std::function<void()>> tasks_;       ///< Queue of tasks to be executed

    std::mutex queue_mutex_;                        ///< Mutex to protect the task queue
    std::condition_variable condition_;             ///< Condition variable to signal worker threads
    bool stop_;                                     ///< Flag to indicate that the pool should stop
};

// Template method definition must be in the header file
template<typename F, typename... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;

    // Create a packaged task that wraps the function and its arguments
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    // Get the future result of the task
    std::future<return_type> res = task->get_future();

    // Lock the queue mutex
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);

        // Don't allow enqueueing after stopping the pool
        if (stop_) {
            throw std::runtime_error("Enqueue on stopped ThreadPool");
        }

        // Add the task to the queue
        tasks_.emplace([task]() { (*task)(); });
    }

    // Notify one waiting thread that a task is available
    condition_.notify_one();

    // Return the future result
    return res;
}

} // namespace chat_room

#endif // THREAD_POOL_H
