#include "tcp_file_server.h"
#include "file_transfer_protocol.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <thread>
#include <chrono>

namespace tcp_file_transfer {

TcpFileServer::TcpFileServer(int port, const std::string& file_storage_path, int timeout_seconds)
    : port_(port), file_storage_path_(file_storage_path), stop_(false), server_socket_(-1), thread_pool_(4), timeout_seconds_(timeout_seconds) {
    // Create the file storage directory if it doesn't exist
    std::filesystem::create_directories(file_storage_path_);
}

TcpFileServer::~TcpFileServer() {
    Stop();
}

bool TcpFileServer::Start() {
    // Create server socket
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }

    // Allow reuse of address
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Failed to set socket options: " << strerror(errno) << std::endl;
        close(server_socket_);
        server_socket_ = -1;
        return false;
    }

    // Bind socket to address and port
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket: " << strerror(errno) << std::endl;
        close(server_socket_);
        server_socket_ = -1;
        return false;
    }

    // Listen for incoming connections
    if (listen(server_socket_, 10) < 0) {
        std::cerr << "Failed to listen on socket: " << strerror(errno) << std::endl;
        close(server_socket_);
        server_socket_ = -1;
        return false;
    }

    std::cout << "TCP file server listening on port " << port_ << ", storing files in " << file_storage_path_ << std::endl;

    // Main server loop
    while (!stop_) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        // Accept a new connection (with a timeout to check stop_ flag)
        // Note: We can't use accept() with a timeout directly, so we'll use a separate approach
        // For simplicity in this implementation, we'll just accept and then check stop_
        // A more robust implementation might use select() or poll() with a timeout
        int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            if (!stop_) {
                std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
            }
            continue;
        }

        std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) 
                  << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Handle the connection in a thread pool
        thread_pool_.Enqueue([this, client_socket]() {
            HandleClient(client_socket);
        });
    }

    std::cout << "Server has stopped." << std::endl;
    return true;
}

void TcpFileServer::Stop() {
    if (!stop_) {
        stop_ = true;

        // Close the server socket to unblock accept()
        if (server_socket_ >= 0) {
            close(server_socket_);
            server_socket_ = -1;
        }
    }
}

void TcpFileServer::HandleClient(int client_socket) {
    // Process messages from the client
    ProcessMessage(client_socket);

    // Close the client socket
    close(client_socket);
    std::cout << "Client connection closed." << std::endl;
}

void TcpFileServer::ProcessMessage(int client_socket) {
    // Set up timeout using select
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_socket, &read_fds);
    
    struct timeval timeout;
    timeout.tv_sec = timeout_seconds_;
    timeout.tv_usec = 0;
    
    int select_result = select(client_socket + 1, &read_fds, NULL, NULL, &timeout);
    if (select_result <= 0) {
        if (select_result < 0) {
            std::cerr << "Error in select: " << strerror(errno) << std::endl;
        } else {
            std::cerr << "Timeout while receiving message header" << std::endl;
        }
        return;
    }
    
    // Read the message header (type and length)
    uint32_t header[2];
    ssize_t bytes_received = recv(client_socket, header, sizeof(header), 0);
    if (bytes_received <= 0) {
        if (bytes_received < 0) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
        }
        return;
    }

    if (bytes_received != sizeof(header)) {
        std::cerr << "Error receiving header: received " << bytes_received << " bytes, expected " << sizeof(header) << std::endl;
        return;
    }

    uint32_t type = header[0];
    uint32_t length = header[1];

    // Check if the message is too large
    if (length > MAX_FILE_SIZE + 1024) { // Adding some buffer for filename
        std::cerr << "Message too large: " << length << " bytes" << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, "", {'M', 'e', 's', 's', 'a', 'g', 'e', ' ', 't', 'o', 'o', ' ', 'l', 'a', 'r', 'g', 'e'});
        return;
    }

    // Set up timeout for reading the rest of the message
    select_result = select(client_socket + 1, &read_fds, NULL, NULL, &timeout);
    if (select_result <= 0) {
        if (select_result < 0) {
            std::cerr << "Error in select: " << strerror(errno) << std::endl;
        } else {
            std::cerr << "Timeout while receiving message data" << std::endl;
        }
        return;
    }

    // Read the rest of the message
    std::vector<char> buffer(length);
    bytes_received = recv(client_socket, buffer.data(), length, 0);
    if (bytes_received <= 0) {
        if (bytes_received < 0) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
        }
        return;
    }

    if (bytes_received != length) {
        std::cerr << "Error receiving data: received " << bytes_received << " bytes, expected " << length << std::endl;
        return;
    }

    // Find the null terminator for the filename
    const char* filename_end = std::find(buffer.data(), buffer.data() + length, '\0');
    if (filename_end == buffer.data() + length) {
        // No null terminator found
        std::cerr << "Invalid message format: no null terminator for filename" << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, "", {'I', 'n', 'v', 'a', 'l', 'i', 'd', ' ', 'm', 'e', 's', 's', 'a', 'g', 'e', ' ', 'f', 'o', 'r', 'm', 'a', 't'});
        return;
    }

    // Extract filename
    std::string filename(buffer.data(), filename_end - buffer.data());

    // Extract data
    size_t data_size = length - (filename_end + 1 - buffer.data());
    std::vector<char> data(filename_end + 1, filename_end + 1 + data_size);

    // Handle the message based on its type
    switch (type) {
        case MSG_TYPE_UPLOAD_REQUEST:
            HandleUploadRequest(client_socket, filename, data);
            break;
        case MSG_TYPE_DOWNLOAD_REQUEST:
            HandleDownloadRequest(client_socket, filename);
            break;
        default:
            std::cerr << "Unknown message type: " << type << std::endl;
            SendResponse(client_socket, MSG_TYPE_ERROR, "", {'U', 'n', 'k', 'n', 'o', 'w', 'n', ' ', 'm', 'e', 's', 's', 'a', 'g', 'e', ' ', 't', 'y', 'p', 'e'});
            break;
    }
}

void TcpFileServer::HandleUploadRequest(int client_socket, const std::string& filename, const std::vector<char>& data) {
    std::string file_path = ResolveFilePath(filename);
    if (file_path.empty()) {
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'I', 'n', 'v', 'a', 'l', 'i', 'd', ' ', 'f', 'i', 'l', 'e', 'n', 'a', 'm', 'e'});
        return;
    }

    // Save the file
    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << file_path << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'F', 'a', 'i', 'l', 'e', 'd', ' ', 't', 'o', ' ', 'o', 'p', 'e', 'n', ' ', 'f', 'i', 'l', 'e'});
        return;
    }

    file.write(data.data(), data.size());
    if (!file) {
        std::cerr << "Failed to write file: " << file_path << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'F', 'a', 'i', 'l', 'e', 'd', ' ', 't', 'o', ' ', 'w', 'r', 'i', 't', 'e', ' ', 'f', 'i', 'l', 'e'});
        return;
    }

    file.close();

    std::cout << "File uploaded successfully: " << file_path << " (" << data.size() << " bytes)" << std::endl;

    // Send success response
    SendResponse(client_socket, MSG_TYPE_UPLOAD_RESPONSE, filename, {});
}

void TcpFileServer::HandleDownloadRequest(int client_socket, const std::string& filename) {
    std::string file_path = ResolveFilePath(filename);
    if (file_path.empty()) {
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'I', 'n', 'v', 'a', 'l', 'i', 'd', ' ', 'f', 'i', 'l', 'e', 'n', 'a', 'm', 'e'});
        return;
    }

    // Check if file exists
    if (!std::filesystem::exists(file_path)) {
        std::cerr << "File not found: " << file_path << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'F', 'i', 'l', 'e', ' ', 'n', 'o', 't', ' ', 'f', 'o', 'u', 'n', 'd'});
        return;
    }

    // Check if it's a regular file
    if (!std::filesystem::is_regular_file(file_path)) {
        std::cerr << "Not a regular file: " << file_path << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'N', 'o', 't', ' ', 'a', ' ', 'r', 'e', 'g', 'u', 'l', 'a', 'r', ' ', 'f', 'i', 'l', 'e'});
        return;
    }

    // Check file size
    size_t file_size = std::filesystem::file_size(file_path);
    if (file_size > MAX_FILE_SIZE) {
        std::cerr << "File too large: " << file_path << " (" << file_size << " bytes)" << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'F', 'i', 'l', 'e', ' ', 't', 'o', 'o', ' ', 'l', 'a', 'r', 'g', 'e'});
        return;
    }

    // Read the file
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << file_path << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'F', 'a', 'i', 'l', 'e', 'd', ' ', 't', 'o', ' ', 'o', 'p', 'e', 'n', ' ', 'f', 'i', 'l', 'e'});
        return;
    }

    std::vector<char> data(file_size);
    file.read(data.data(), file_size);
    if (!file) {
        std::cerr << "Failed to read file: " << file_path << std::endl;
        SendResponse(client_socket, MSG_TYPE_ERROR, filename, {'F', 'a', 'i', 'l', 'e', 'd', ' ', 't', 'o', ' ', 'r', 'e', 'a', 'd', ' ', 'f', 'i', 'l', 'e'});
        return;
    }

    file.close();

    std::cout << "File downloaded successfully: " << file_path << " (" << file_size << " bytes)" << std::endl;

    // Send success response
    SendResponse(client_socket, MSG_TYPE_DOWNLOAD_RESPONSE, filename, data);
}

bool TcpFileServer::SendResponse(int client_socket, uint32_t type, const std::string& filename, const std::vector<char>& data) {
    // Create the message
    Message msg;
    msg.type = type;
    msg.filename = filename;
    msg.data = data;

    // Serialize the message
    std::vector<char> buffer = SerializeMessage(msg);

    // Send the message
    ssize_t total_bytes_sent = 0;
    size_t total_bytes_to_send = buffer.size();

    while (total_bytes_sent < total_bytes_to_send) {
        ssize_t bytes_sent = send(client_socket, buffer.data() + total_bytes_sent, 
                                  total_bytes_to_send - total_bytes_sent, 0);
        if (bytes_sent <= 0) {
            if (bytes_sent < 0) {
                std::cerr << "Error sending data: " << strerror(errno) << std::endl;
            }
            return false;
        }
        total_bytes_sent += bytes_sent;
    }

    return true;
}

std::string TcpFileServer::ResolveFilePath(const std::string& filename) const {
    // Basic filename validation and security check
    // This prevents directory traversal attacks like ../../etc/passwd
    if (filename.empty() || filename[0] == '/' || filename.find("..") != std::string::npos) {
        return ""; // Invalid filename
    }

    // Resolve the path relative to file_storage_path_
    std::filesystem::path root_path(file_storage_path_);
    std::filesystem::path resolved_path = root_path / filename;

    // Canonicalize the path to resolve any '..' or '.'
    resolved_path = std::filesystem::weakly_canonical(resolved_path);

    // Check if the resolved path is within the file_storage_path_ directory
    std::filesystem::path canonical_root = std::filesystem::weakly_canonical(root_path);
    if (resolved_path.string().find(canonical_root.string()) != 0) {
        return ""; // Path traversal detected
    }

    return resolved_path.string();
}

} // namespace tcp_file_transfer
