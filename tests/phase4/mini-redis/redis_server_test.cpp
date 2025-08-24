/**
 * @file redis_server_test.cpp
 * @brief Unit tests for the Redis server.
 */

#include "redis_server.h"
#include "redis_client.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

namespace redis {

class RedisServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up code here if needed
    }

    void TearDown() override {
        // Clean up code here if needed
    }

    static const int TEST_PORT = 6380; // Use a different port to avoid conflicts
};

TEST_F(RedisServerTest, ServerStartAndStop) {
    // Test starting and stopping the server
    RedisServer server(TEST_PORT);

    // Start the server in a separate thread
    std::thread server_thread([&server]() {
        server.Start();
    });

    // Give the server some time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Stop the server
    server.Stop();

    // Wait for the server thread to complete
    if (server_thread.joinable()) {
        server_thread.join();
    }

    // If we reach this point, the test passed
    SUCCEED();
}

TEST_F(RedisServerTest, ClientConnectAndPing) {
    // Test client connecting to server and sending PING command
    RedisServer server(TEST_PORT);

    // Start the server in a separate thread
    std::thread server_thread([&server]() {
        server.Start();
    });

    // Give the server some time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Create a client and connect to the server
    RedisClient client;
    EXPECT_TRUE(client.Connect("127.0.0.1", TEST_PORT));

    // Send a PING command
    EXPECT_TRUE(client.Ping());

    // Disconnect the client
    client.Disconnect();

    // Stop the server
    server.Stop();

    // Wait for the server thread to complete
    if (server_thread.joinable()) {
        server_thread.join();
    }
}

TEST_F(RedisServerTest, SetAndGet) {
    // Test setting and getting a value through the server
    RedisServer server(TEST_PORT);

    // Start the server in a separate thread
    std::thread server_thread([&server]() {
        server.Start();
    });

    // Give the server some time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Create a client and connect to the server
    RedisClient client;
    EXPECT_TRUE(client.Connect("127.0.0.1", TEST_PORT));

    // Set a key-value pair
    EXPECT_TRUE(client.Set("test_key", "test_value"));

    // Get the value
    auto result = client.Get("test_key");
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "test_value");

    // Disconnect the client
    client.Disconnect();

    // Stop the server
    server.Stop();

    // Wait for the server thread to complete
    if (server_thread.joinable()) {
        server_thread.join();
    }
}

} // namespace redis
