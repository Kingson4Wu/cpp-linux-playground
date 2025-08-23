/**
 * @file main.cpp
 * @brief Main entry point for the HTTP static file server.
 *
 * This application implements a simple HTTP 1.1 static file server
 * that can serve files from a specified directory.
 *
 * How to Run with Docker (builds and runs automatically):
 *   ./scripts/docker-dev.sh exec
 *   # Inside the container, navigate to the build directory and run the example
 *   cd build/phase3/http-server
 *   ./http_server [PORT] [WEB_ROOT]
 *
 * How to Compile and Run manually in Docker:
 *   1. Enter the Docker container:
 *      ./scripts/docker-dev.sh exec
 *   2. Inside the container, create a build directory and compile:
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *   3. Run the executable:
 *      ./phase3/http-server/http_server [PORT] [WEB_ROOT]
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase3/http-server/http_server [PORT] [WEB_ROOT]
 *
 * Usage:
 *   ./build/phase3/http-server/http_server [PORT] [WEB_ROOT]
 *
 * Examples:
 *   # Run the server on port 8080, serving files from the current directory
 *   ./build/phase3/http-server/http_server 8080 .
 *
 *   # Run the server on port 8000, serving files from /var/www
 *   ./build/phase3/http-server/http_server 8000 /var/www
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "http_server") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "http_connection_handler.h"
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

// Global variable to store the server socket for signal handling
static int server_socket = -1;

// Signal handler for graceful shutdown
void SignalHandler(int signal) {
    std::cout << "
Received signal " << signal << ". Shutting down server..." << std::endl;
    if (server_socket >= 0) {
        close(server_socket);
        server_socket = -1;
    }
}

int main(int argc, char* argv[]) {
    // Default values
    int port = 8080;
    std::string web_root = ".";

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

    if (argc > 2) {
        web_root = argv[2];
    }

    // Register signal handlers for graceful shutdown
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // Allow reuse of address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Failed to set socket options" << std::endl;
        close(server_socket);
        return 1;
    }

    // Bind socket to address and port
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(server_socket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "HTTP server listening on port " << port << ", serving files from " << web_root << std::endl;

    // Main server loop
    while (server_socket >= 0) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        // Accept a new connection
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            if (server_socket < 0) {
                // Server was shut down
                break;
            }
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) 
                  << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Handle the connection
        // For now, we handle it synchronously. In a more advanced version, we would use a thread pool.
        http_server::HttpConnectionHandler handler(client_socket, web_root);
        handler.Handle();

        // Close the client socket (HttpConnectionHandler's destructor will do this)
        // close(client_socket);
    }

    std::cout << "Server has stopped." << std::endl;
    return 0;
}
