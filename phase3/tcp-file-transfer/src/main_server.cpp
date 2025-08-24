/**
 * @file main_server.cpp
 * @brief Main entry point for the TCP file transfer server.
 *
 * This application implements a simple TCP file transfer server
 * that can handle file uploads and downloads.
 *
 * How to Run with Docker (builds and runs automatically):
 *   ./scripts/docker-dev.sh exec
 *   # Inside the container, navigate to the build directory and run the example
 *   cd build/phase3/tcp-file-transfer
 *   ./tcp_file_server [PORT] [STORAGE_PATH]
 *
 * How to Compile and Run manually in Docker:
 *   1. Enter the Docker container:
 *      ./scripts/docker-dev.sh exec
 *   2. Inside the container, create a build directory and compile:
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *   3. Run the executable:
 *      ./phase3/tcp-file-transfer/tcp_file_server [PORT] [STORAGE_PATH]
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase3/tcp-file-transfer/tcp_file_server [PORT] [STORAGE_PATH]
 *
 * Usage:
 *   ./build/phase3/tcp-file-transfer/tcp_file_server [PORT] [STORAGE_PATH]
 *
 * Examples:
 *   # Run the server on port 8080, storing files in the current directory
 *   ./build/phase3/tcp-file-transfer/tcp_file_server 8080 .
 *
 *   # Run the server on port 9000, storing files in /var/files
 *   ./build/phase3/tcp-file-transfer/tcp_file_server 9000 /var/files
 *
 * Note: This server works with the TCP file transfer client, which now supports
 * uploading and downloading files without specifying a new filename. When using
 * the client with this feature:
 *   - For uploads, if no remote filename is specified, the client will use the
 *     basename of the local file as the remote filename.
 *   - For downloads, if no local filename is specified, the client will use the
 *     remote filename as the local filename.
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "tcp_file_server") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "tcp_file_server.h"
#include <iostream>
#include <cstdlib>
#include <csignal>

// Global pointer to the server instance for signal handling
static tcp_file_transfer::TcpFileServer* g_server = nullptr;

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
    int port = 8080;
    std::string storage_path = ".";

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
        storage_path = argv[2];
    }

    // Create the server
    tcp_file_transfer::TcpFileServer server(port, storage_path);
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
