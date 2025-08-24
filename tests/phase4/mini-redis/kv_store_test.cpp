/**
 * @file kv_store_test.cpp
 * @brief Unit tests for the Redis key-value store.
 */

#include "kv_store.h"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

namespace redis {

class KVStoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up code here if needed
    }

    void TearDown() override {
        // Clean up code here if needed
    }

    KVStore store;
};

TEST_F(KVStoreTest, SetAndGet) {
    // Test setting and getting a value
    EXPECT_TRUE(store.Set("key1", "value1"));
    auto result = store.Get("key1");
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "value1");
}

TEST_F(KVStoreTest, GetNonExistentKey) {
    // Test getting a non-existent key
    auto result = store.Get("nonexistent");
    EXPECT_FALSE(result.has_value());
}

TEST_F(KVStoreTest, Delete) {
    // Test deleting a key
    store.Set("key1", "value1");
    EXPECT_TRUE(store.Delete("key1"));
    EXPECT_FALSE(store.Get("key1").has_value());
}

TEST_F(KVStoreTest, DeleteNonExistentKey) {
    // Test deleting a non-existent key
    EXPECT_FALSE(store.Delete("nonexistent"));
}

TEST_F(KVStoreTest, Exists) {
    // Test checking if a key exists
    store.Set("key1", "value1");
    EXPECT_TRUE(store.Exists("key1"));
    EXPECT_FALSE(store.Exists("nonexistent"));
}

TEST_F(KVStoreTest, Size) {
    // Test getting the size of the store
    EXPECT_EQ(store.Size(), 0);
    store.Set("key1", "value1");
    EXPECT_EQ(store.Size(), 1);
    store.Set("key2", "value2");
    EXPECT_EQ(store.Size(), 2);
    store.Delete("key1");
    EXPECT_EQ(store.Size(), 1);
}

TEST_F(KVStoreTest, Concurrency) {
    // Test concurrent access to the store
    const int num_threads = 10;
    const int num_operations = 1000;

    std::vector<std::thread> threads;

    // Launch threads that perform operations on the store
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, i, num_operations]() {
            for (int j = 0; j < num_operations; ++j) {
                std::string key = "key" + std::to_string(i * num_operations + j);
                std::string value = "value" + std::to_string(i * num_operations + j);

                // Set the key-value pair
                EXPECT_TRUE(store.Set(key, value));

                // Get the value and verify it
                auto result = store.Get(key);
                EXPECT_TRUE(result.has_value());
                EXPECT_EQ(result.value(), value);

                // Check if the key exists
                EXPECT_TRUE(store.Exists(key));
            }
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Verify the final state
    EXPECT_EQ(store.Size(), num_threads * num_operations);
}

} // namespace redis
