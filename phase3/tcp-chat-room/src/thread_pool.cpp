#include "thread_pool.h"
#include <stdexcept>

namespace chat_room {

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    // Create the worker threads
    for (size_t i = 0; i < num_threads; ++i) {
        threads_.emplace_back([this] {
            // Worker thread loop
            for (;;) {
                std::function<void()> task;

                // Lock the queue mutex
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex_);

                    // Wait until there is a task or the pool is stopped
                    this->condition_.wait(lock, [this] { 
                        return this->stop_ || !this->tasks_.empty(); 
                    });

                    // If the pool is stopped and there are no more tasks, exit the thread
                    if (this->stop_ && this->tasks_.empty()) {
                        return;
                    }

                    // Get the next task from the queue
                    task = std::move(this->tasks_.front());
                    this->tasks_.pop();
                }

                // Execute the task
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    // Lock the queue mutex
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }

    // Notify all worker threads to wake up
    condition_.notify_all();

    // Wait for all worker threads to finish
    for (std::thread &worker : threads_) {
        worker.join();
    }
}

} // namespace chat_room
