#include "tcp_file_client.h"
#include "file_transfer_protocol.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <filesystem>

namespace tcp_file_transfer {

TcpFileClient::TcpFileClient() : client_socket_(-1) {}

TcpFileClient::~TcpFileClient() {
    Disconnect();
}

bool TcpFileClient::Connect(const std::string& server_ip, int port) {
    // Create client socket
    client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }

    // Connect to the server
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported: " << server_ip << std::endl;
        close(client_socket_);
        client_socket_ = -1;
        return false;
    }

    if (connect(client_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        close(client_socket_);
        client_socket_ = -1;
        return false;
    }

    std::cout << "Connected to server " << server_ip << ":" << port << std::endl;
    return true;
}

void TcpFileClient::Disconnect() {
    if (client_socket_ >= 0) {
        close(client_socket_);
        client_socket_ = -1;
        std::cout << "Disconnected from server." << std::endl;
    }
}

bool TcpFileClient::UploadFile(const std::string& local_file_path, const std::string& remote_filename) {
    if (client_socket_ < 0) {
        std::cerr << "Not connected to a server." << std::endl;
        return false;
    }

    // Check if local file exists
    if (!std::filesystem::exists(local_file_path)) {
        std::cerr << "Local file not found: " << local_file_path << std::endl;
        return false;
    }

    // Check if it's a regular file
    if (!std::filesystem::is_regular_file(local_file_path)) {
        std::cerr << "Not a regular file: " << local_file_path << std::endl;
        return false;
    }

    // Check file size
    size_t file_size = std::filesystem::file_size(local_file_path);
    if (file_size > MAX_FILE_SIZE) {
        std::cerr << "File too large: " << local_file_path << " (" << file_size << " bytes)" << std::endl;
        return false;
    }

    // Read the file
    std::ifstream file(local_file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << local_file_path << std::endl;
        return false;
    }

    std::vector<char> data(file_size);
    file.read(data.data(), file_size);
    if (!file) {
        std::cerr << "Failed to read file: " << local_file_path << std::endl;
        return false;
    }

    file.close();

    // Send upload request
    if (!SendMessage(MSG_TYPE_UPLOAD_REQUEST, remote_filename, data)) {
        std::cerr << "Failed to send upload request." << std::endl;
        return false;
    }

    // Receive response
    uint32_t type;
    std::string filename;
    std::vector<char> response_data;
    if (!ReceiveMessage(type, filename, response_data)) {
        std::cerr << "Failed to receive response." << std::endl;
        return false;
    }

    if (type == MSG_TYPE_UPLOAD_RESPONSE) {
        std::cout << "File uploaded successfully: " << local_file_path << " -> " << remote_filename << std::endl;
        return true;
    } else if (type == MSG_TYPE_ERROR) {
        std::cerr << "Server error: " << std::string(response_data.begin(), response_data.end()) << std::endl;
        return false;
    } else {
        std::cerr << "Unexpected response type: " << type << std::endl;
        return false;
    }
}

bool TcpFileClient::DownloadFile(const std::string& remote_filename, const std::string& local_file_path) {
    if (client_socket_ < 0) {
        std::cerr << "Not connected to a server." << std::endl;
        return false;
    }

    // Send download request
    if (!SendMessage(MSG_TYPE_DOWNLOAD_REQUEST, remote_filename, {})) {
        std::cerr << "Failed to send download request." << std::endl;
        return false;
    }

    // Receive response
    uint32_t type;
    std::string filename;
    std::vector<char> data;
    if (!ReceiveMessage(type, filename, data)) {
        std::cerr << "Failed to receive response." << std::endl;
        return false;
    }

    if (type == MSG_TYPE_DOWNLOAD_RESPONSE) {
        // Save the file
        std::ofstream file(local_file_path, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file for writing: " << local_file_path << std::endl;
            return false;
        }

        file.write(data.data(), data.size());
        if (!file) {
            std::cerr << "Failed to write file: " << local_file_path << std::endl;
            return false;
        }

        file.close();

        std::cout << "File downloaded successfully: " << remote_filename << " -> " << local_file_path 
                  << " (" << data.size() << " bytes)" << std::endl;
        return true;
    } else if (type == MSG_TYPE_ERROR) {
        std::cerr << "Server error: " << std::string(data.begin(), data.end()) << std::endl;
        return false;
    } else {
        std::cerr << "Unexpected response type: " << type << std::endl;
        return false;
    }
}

bool TcpFileClient::SendMessage(uint32_t type, const std::string& filename, const std::vector<char>& data) {
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
        ssize_t bytes_sent = send(client_socket_, buffer.data() + total_bytes_sent, 
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

bool TcpFileClient::ReceiveMessage(uint32_t& type, std::string& filename, std::vector<char>& data) {
    // Read the message header (type and length)
    uint32_t header[2];
    ssize_t bytes_received = recv(client_socket_, header, sizeof(header), 0);
    if (bytes_received <= 0) {
        if (bytes_received < 0) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
        }
        return false;
    }

    if (bytes_received != sizeof(header)) {
        std::cerr << "Error receiving header: received " << bytes_received << " bytes, expected " << sizeof(header) << std::endl;
        return false;
    }

    type = header[0];
    uint32_t length = header[1];

    // Check if the message is too large
    if (length > MAX_FILE_SIZE + 1024) { // Adding some buffer for filename
        std::cerr << "Message too large: " << length << " bytes" << std::endl;
        return false;
    }

    // Read the rest of the message
    std::vector<char> buffer(length);
    bytes_received = recv(client_socket_, buffer.data(), length, 0);
    if (bytes_received <= 0) {
        if (bytes_received < 0) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
        }
        return false;
    }

    if (bytes_received != length) {
        std::cerr << "Error receiving data: received " << bytes_received << " bytes, expected " << length << std::endl;
        return false;
    }

    // Find the null terminator for the filename
    const char* filename_end = std::find(buffer.data(), buffer.data() + length, '\0');
    if (filename_end == buffer.data() + length) {
        // No null terminator found
        std::cerr << "Invalid message format: no null terminator for filename" << std::endl;
        return false;
    }

    // Extract filename
    filename = std::string(buffer.data(), filename_end - buffer.data());

    // Extract data
    size_t data_size = length - (filename_end + 1 - buffer.data());
    data.assign(filename_end + 1, filename_end + 1 + data_size);

    return true;
}

} // namespace tcp_file_transfer
