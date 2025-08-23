/*
 * logger_example.cpp - Example usage of the multi-threaded logger
 *
 * This file demonstrates how to use the multi-threaded logger library.
 *
 * How to Run with Docker (builds and runs automatically):
 *   ./scripts/docker-dev.sh run-logger
 *
 * How to Compile and Run manually in Docker:
 *   1. Enter the Docker container:
 *      ./scripts/docker-dev.sh exec
 *   2. Inside the container, create a build directory and compile:
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *   3. Run the executable:
 *      ./phase1/logger/logger_example
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase1/logger/logger_example
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "logger_example") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

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
