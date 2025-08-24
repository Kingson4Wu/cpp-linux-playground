/**
 * @file main_client.cpp
 * @brief Main entry point for the TCP file transfer client.
 *
 * This application implements a simple TCP file transfer client
 * that can upload and download files to/from the server.
 *
 * How to Run with Docker (builds and runs automatically):
 *   ./scripts/docker-dev.sh exec
 *   # Inside the container, navigate to the build directory and run the example
 *   cd build/phase3/tcp-file-transfer
 *   ./tcp_file_client [SERVER_IP] [PORT] [COMMAND] [ARGS...]
 *
 * How to Compile and Run manually in Docker:
 *   1. Enter the Docker container:
 *      ./scripts/docker-dev.sh exec
 *   2. Inside the container, create a build directory and compile:
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *   3. Run the executable:
 *      ./phase3/tcp-file-transfer/tcp_file_client [SERVER_IP] [PORT] [COMMAND] [ARGS...]
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase3/tcp-file-transfer/tcp_file_client [SERVER_IP] [PORT] [COMMAND] [ARGS...]
 *
 * Usage:
 *   ./build/phase3/tcp-file-transfer/tcp_file_client [SERVER_IP] [PORT] upload [LOCAL_FILE] [REMOTE_FILENAME]
 *   ./build/phase3/tcp-file-transfer/tcp_file_client [SERVER_IP] [PORT] download [REMOTE_FILENAME] [LOCAL_FILE]
 *
 *   If REMOTE_FILENAME or LOCAL_FILE is omitted, the default filename will be used.
 *   For uploads, the default remote filename is the basename of the local file.
 *   For downloads, the default local filename is the remote filename.
 *
 * Examples:
 *   # Upload a file with a custom remote name
 *   ./build/phase3/tcp-file-transfer/tcp_file_client 127.0.0.1 8080 upload /path/to/local/file.txt remote_file.txt
 *
 *   # Upload a file with the default remote name (file.txt)
 *   ./build/phase3/tcp-file-transfer/tcp_file_client 127.0.0.1 8080 upload /path/to/local/file.txt
 *
 *   # Download a file with a custom local name
 *   ./build/phase3/tcp-file-transfer/tcp_file_client 127.0.0.1 8080 download remote_file.txt /path/to/local/file.txt
 *
 *   # Download a file with the default local name (remote_file.txt)
 *   ./build/phase3/tcp-file-transfer/tcp_file_client 127.0.0.1 8080 download remote_file.txt
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "tcp_file_client") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "tcp_file_client.h"
#include <iostream>
#include <cstdlib>
#include <filesystem>

// Extract the basename of a file path
std::string GetBasename(const std::string& path) {
    return std::filesystem::path(path).filename().string();
}

int main(int argc, char* argv[]) {
    // Parse command line arguments
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " [SERVER_IP] [PORT] [COMMAND] [ARGS...]" << std::endl;
        std::cerr << "Commands:" << std::endl;
        std::cerr << "  upload [LOCAL_FILE] [REMOTE_FILENAME]" << std::endl;
        std::cerr << "  download [REMOTE_FILENAME] [LOCAL_FILE]" << std::endl;
        return 1;
    }

    std::string server_ip = argv[1];

    int port;
    try {
        port = std::stoi(argv[2]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Invalid port number. Must be between 1 and 65535." << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Invalid port number: " << argv[2] << std::endl;
        return 1;
    }

    std::string command = argv[3];

    // Create the client
    tcp_file_transfer::TcpFileClient client;

    // Connect to the server
    if (!client.Connect(server_ip, port)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    // Execute the command
    bool success = false;
    if (command == "upload" && (argc == 5 || argc == 6)) {
        std::string local_file = argv[4];
        std::string remote_filename = (argc == 6) ? argv[5] : GetBasename(local_file);
        success = client.UploadFile(local_file, remote_filename);
    } else if (command == "download" && (argc == 5 || argc == 6)) {
        std::string remote_filename = argv[4];
        std::string local_file = (argc == 6) ? argv[5] : remote_filename;
        success = client.DownloadFile(remote_filename, local_file);
    } else {
        std::cerr << "Invalid command or arguments." << std::endl;
        std::cerr << "Usage: " << argv[0] << " [SERVER_IP] [PORT] [COMMAND] [ARGS...]" << std::endl;
        std::cerr << "Commands:" << std::endl;
        std::cerr << "  upload [LOCAL_FILE] [REMOTE_FILENAME]" << std::endl;
        std::cerr << "  download [REMOTE_FILENAME] [LOCAL_FILE]" << std::endl;
        client.Disconnect();
        return 1;
    }

    // Disconnect from the server
    client.Disconnect();

    if (success) {
        std::cout << "Command executed successfully." << std::endl;
        return 0;
    } else {
        std::cerr << "Command failed." << std::endl;
        return 1;
    }
}