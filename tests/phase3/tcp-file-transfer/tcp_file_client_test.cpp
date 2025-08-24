/**
 * @file tcp_file_client_test.cpp
 * @brief Unit tests for the TCP file transfer client.
 */

#include "tcp_file_client.h"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

namespace tcp_file_transfer {

class TcpFileClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary file for testing
        test_file_path_ = "test_file.txt";
        std::ofstream file(test_file_path_);
        file << "This is a test file for TCP file transfer client.";
        file.close();
    }

    void TearDown() override {
        // Clean up the temporary file
        if (std::filesystem::exists(test_file_path_)) {
            std::filesystem::remove(test_file_path_);
        }
    }

    std::string test_file_path_;
};

// Note: These tests require a running server, so they are more like integration tests
// For unit testing the client logic, we would need to mock the network communication

} // namespace tcp_file_transfer
