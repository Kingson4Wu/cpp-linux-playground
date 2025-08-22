/**
 * @file server_main.cpp
 * @brief Main entry point for the TCP chat server.
 *
 * This application implements a multi-threaded TCP chat server
 * that allows multiple clients to connect and exchange messages
 * in real-time. It uses a thread pool for efficient client handling.
 *
 * How to Compile and Run with Docker:
 *   1. Build and run chat_server in Docker container:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase3/tcp-chat-room/chat_server [PORT]
 *
 *   2. Or, to build and run manually in Docker:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase3/tcp-chat-room/chat_server [PORT]
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase3/tcp-chat-room/chat_server [PORT]
 *
 * Usage:
 *   ./build/phase3/tcp-chat-room/chat_server [PORT]
 *
 * Examples:
 *   # Run the server on port 8080
 *   ./build/phase3/tcp-chat-room/chat_server 8080
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "chat_server") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "chat_server.h"
#include <iostream>
#include <cstdlib>
#include <csignal>

// Global pointer to the server instance for signal handling
chat_room::ChatServer* g_server = nullptr;

// Signal handler for graceful shutdown
void SignalHandler(int signal) {
    std::cout << "
Received signal " << signal << ". Shutting down server..." << std::endl;
    if (g_server) {
        g_server->Stop();
    }
}

int main(int argc, char* argv[]) {
    // Default port
    int port = 8080;

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

    // Register signal handlers for graceful shutdown
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    try {
        // Create the chat server
        // Use 4 threads in the thread pool by default
        chat_room::ChatServer server(port, 4);
        g_server = &server;

        std::cout << "Starting TCP chat server on port " << port << "..." << std::endl;

        // Run the server
        server.Run();

        std::cout << "Server has stopped." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
