/**
 * @file main_server.cpp
 * @brief Main entry point for the mini-Redis server.
 *
 * This application implements a simple Redis-compatible server
 * that can handle client connections and execute Redis commands.
 *
 * How to Compile and Run:
 *   1. From the project root directory:
 *      cmake -S . -B build
 *      cmake --build build -- -j
 *   2. Run the server:
 *      ./build/phase4/mini-redis/redis_server [PORT]
 *
 * Usage:
 *   ./build/phase4/mini-redis/redis_server [PORT]
 *
 * Examples:
 *   # Run the server on the default port (6379)
 *   ./build/phase4/mini-redis/redis_server
 *
 *   # Run the server on port 6380
 *   ./build/phase4/mini-redis/redis_server 6380
 *
 * Supported Commands:
 *   - PING: Ping the server
 *   - SET key value: Set a key-value pair
 *   - GET key: Retrieve the value for a key
 *   - DEL key: Delete a key
 *   - EXISTS key: Check if a key exists
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "redis_server") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "redis_server.h"
#include <iostream>
#include <cstdlib>
#include <csignal>

// Global pointer to the server instance for signal handling
static redis::RedisServer* g_server = nullptr;

// Signal handler for graceful shutdown
void SignalHandler(int signal) {
    std::cout << "
Received signal " << signal << ". Shutting down server..." << std::endl;
    if (g_server) {
        g_server->Stop();
    }
}

int main(int argc, char* argv[]) {
    // Default values
    int port = 6379;

    // Parse command line arguments
    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
            if (port <= 0 || port > 65535) {
                std::cerr << "Invalid port number. Must be between 1 and 65535." << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid port number: " << argv[1] << std::endl;
            return 1;
        }
    }

    // Create the server
    redis::RedisServer server(port);
    g_server = &server;

    // Register signal handlers for graceful shutdown
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    // Start the server
    if (!server.Start()) {
        std::cerr << "Failed to start server." << std::endl;
        return 1;
    }

    return 0;
}
