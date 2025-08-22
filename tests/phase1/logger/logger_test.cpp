#include "gtest/gtest.h"
#include "logger.h"
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

// Helper function to read file content
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

TEST(LoggerTest, MultiThreadedLogging) {
    // Ensure the log file is clean before the test
    std::ofstream ofs("app.log", std::ios::out | std::ios::trunc);
    ofs.close();

    Logger::getInstance().log(LogLevel::INFO, "Test started.");

    auto logging_task = [](int id) {
        for (int i = 0; i < 10; ++i) {
            std::string msg = "Log message " + std::to_string(i) + " from thread " + std::to_string(id);
            Logger::getInstance().log(LogLevel::INFO, msg);
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(logging_task, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    // It's tricky to reliably check the log file content immediately after threads join
    // because the logger's worker thread is asynchronous.
    // A small delay can help ensure the worker has had time to write.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    Logger::getInstance().log(LogLevel::INFO, "Test finished.");

    // Another delay to ensure "Test finished" is written.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string log_content = readFile("app.log");

    // Check that the start and end messages are there.
    ASSERT_NE(log_content.find("Test started."), std::string::npos);
    ASSERT_NE(log_content.find("Test finished."), std::string::npos);

    // Check that all threads logged their messages.
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 10; ++j) {
            std::string expected_msg = "Log message " + std::to_string(j) + " from thread " + std::to_string(i);
            ASSERT_NE(log_content.find(expected_msg), std::string::npos);
        }
    }
}
