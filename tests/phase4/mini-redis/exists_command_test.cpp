/**
 * @file exists_command_test.cpp
 * @brief Test for the EXISTS command functionality in the Redis client.
 */

#include "redis_client.h"
#include "redis_server.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

namespace redis {

class ExistsCommandTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up code here if needed
    }

    void TearDown() override {
        // Clean up code here if needed
    }

    static const int TEST_PORT = 6381; // Use a different port to avoid conflicts
};

TEST_F(ExistsCommandTest, ClientExistsMethod) {
    // Test client EXISTS method
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

    // Check existence of a non-existent key
    EXPECT_FALSE(client.Exists("test_key"));

    // Set a key-value pair
    EXPECT_TRUE(client.Set("test_key", "test_value"));

    // Check existence of the key
    EXPECT_TRUE(client.Exists("test_key"));

    // Get the value to verify it's correctly set
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}