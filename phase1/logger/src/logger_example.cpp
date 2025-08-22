#include "logger.h"
#include <thread>
#include <vector>
#include <iostream>

void log_test(int id) {
    Logger::getInstance().log(LogLevel::INFO, "Thread " + std::to_string(id) + " started.");
    for (int i = 0; i < 5; ++i) {
        Logger::getInstance().log(LogLevel::DEBUG, "Thread " + std::to_string(id) + " logging message " + std::to_string(i));
    }
    Logger::getInstance().log(LogLevel::INFO, "Thread " + std::to_string(id) + " finished.");
}

int main() {
    std::cout << "Logging example started. Check app.log for output." << std::endl;
    Logger::getInstance().log(LogLevel::INFO, "Main thread started.");

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(log_test, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    Logger::getInstance().log(LogLevel::INFO, "Main thread finished.");
    std::cout << "Logging example finished." << std::endl;
    return 0;
}
