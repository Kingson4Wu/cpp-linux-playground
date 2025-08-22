/**
 * @file client_main.cpp
 * @brief Main entry point for the TCP chat client.
 *
 * This application implements a TCP chat client that connects to
 * a chat server and allows users to send and receive messages.
 *
 * How to Compile and Run with Docker:
 *   1. Build and run chat_client in Docker container:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase3/tcp-chat-room/chat_client [SERVER_IP] [PORT]
 *
 *   2. Or, to build and run manually in Docker:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase3/tcp-chat-room/chat_client [SERVER_IP] [PORT]
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase3/tcp-chat-room/chat_client [SERVER_IP] [PORT]
 *
 * Usage:
 *   ./build/phase3/tcp-chat-room/chat_client [SERVER_IP] [PORT]
 *
 * Examples:
 *   # Connect to a server running on localhost:8080
 *   ./build/phase3/tcp-chat-room/chat_client 127.0.0.1 8080
 *
 *   # Connect to a server running on a remote host
 *   ./build/phase3/tcp-chat-room/chat_client 192.168.1.100 8080
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "chat_client") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "chat_client.h"
#include <iostream>
#include <cstdlib>
#include <csignal>

// Global pointer to the client instance for signal handling
chat_room::ChatClient* g_client = nullptr;

// Signal handler for graceful shutdown
void SignalHandler(int signal) {
    std::cout << "
Received signal " << signal << ". Disconnecting from server..." << std::endl;
    if (g_client) {
        g_client->Disconnect();
    }
}

int main(int argc, char* argv[]) {
    // Default server IP and port
    std::string server_ip = "127.0.0.1";
    int server_port = 8080;

    // Parse command line arguments
    if (argc > 1) {
        server_ip = argv[1];
    }

    if (argc > 2) {
        try {
            server_port = std::stoi(argv[2]);
            if (server_port <= 0 || server_port > 65535) {
                std::cerr << "Invalid port number. Must be between 1 and 65535." << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid port number: " << argv[2] << std::endl;
            return 1;
        }
    }

    // Register signal handlers for graceful shutdown
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    try {
        // Create the chat client
        chat_room::ChatClient client(server_ip, server_port);
        g_client = &client;

        // Connect to the server
        if (!client.Connect()) {
            std::cerr << "Failed to connect to server " << server_ip << ":" << server_port << std::endl;
            return 1;
        }

        std::cout << "Connected to server " << server_ip << ":" << server_port << std::endl;

        // Run the client
        client.Run();

        std::cout << "Client has disconnected." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
