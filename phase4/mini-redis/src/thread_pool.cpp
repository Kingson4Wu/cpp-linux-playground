#include "redis_server.h"
#include <iostream>
#include <stdexcept>

namespace redis {

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
    // Create the worker threads
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this] {
            // Worker thread loop
            while (true) {
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
    for (std::thread &worker : workers_) {
        worker.join();
    }
}

void ThreadPool::Enqueue(std::function<void()> task) {
    // Lock the queue mutex
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);

        // Don't allow enqueueing after stopping the pool
        if (stop_) {
            throw std::runtime_error("Enqueue on stopped ThreadPool");
        }

        // Add the task to the queue
        tasks_.emplace(task);
    }

    // Notify one waiting thread that a task is available
    condition_.notify_one();
}

} // namespace redis