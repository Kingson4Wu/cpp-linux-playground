/**
 * @file thread_pool_test.cpp
 * @brief Unit tests for the thread pool.
 */

#include "thread_pool.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <atomic>

namespace crawler {

TEST(ThreadPoolTest, CreateAndDestroy) {
    ThreadPool pool(4);
    // If we reach this point, the pool was created and destroyed successfully
    SUCCEED();
}

TEST(ThreadPoolTest, ExecuteTasks) {
    ThreadPool pool(4);
    std::atomic<int> counter(0);
    
    // Enqueue 10 tasks
    for (int i = 0; i < 10; ++i) {
        pool.Enqueue([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter.fetch_add(1);
        });
    }
    
    // Wait a bit for tasks to complete
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // Check that all tasks were executed
    EXPECT_EQ(counter.load(), 10);
}

} // namespace crawler