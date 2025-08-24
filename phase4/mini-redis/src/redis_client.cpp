#include "redis_client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>

namespace redis {

RedisClient::RedisClient() : client_socket_(-1) {}

RedisClient::~RedisClient() {
    Disconnect();
}

bool RedisClient::Connect(const std::string& host, int port) {
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
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported: " << host << std::endl;
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

    std::cout << "Connected to Redis server " << host << ":" << port << std::endl;
    return true;
}

void RedisClient::Disconnect() {
    if (client_socket_ >= 0) {
        close(client_socket_);
        client_socket_ = -1;
    }
}

std::unique_ptr<RESPValue> RedisClient::SendCommand(const std::string& command) {
    if (client_socket_ < 0) {
        std::cerr << "Not connected to a server." << std::endl;
        return nullptr;
    }

    // For simplicity, we'll assume the command is already in RESP format
    if (!SendData(command)) {
        std::cerr << "Failed to send command." << std::endl;
        return nullptr;
    }

    // Receive response
    std::string response_data = ReceiveData();
    if (response_data.empty()) {
        std::cerr << "Failed to receive response." << std::endl;
        return nullptr;
    }

    // Parse response
    return protocol_.Parse(response_data);
}

bool RedisClient::Set(const std::string& key, const std::string& value) {
    // Create SET command in RESP format
    auto array = protocol_.CreateArray();
    array->AddElement(std::move(protocol_.CreateBulkString("SET")));
    array->AddElement(std::move(protocol_.CreateBulkString(key)));
    array->AddElement(std::move(protocol_.CreateBulkString(value)));

    std::string command = protocol_.Serialize(*array);

    auto response = SendCommand(command);
    if (!response) {
        return false;
    }

    // Check if response is a simple string with value "OK"
    if (response->type == RESPType::SIMPLE_STRING) {
        const auto& simple_string = static_cast<const SimpleString&>(*response);
        return simple_string.value == "OK";
    }

    return false;
}

std::optional<std::string> RedisClient::Get(const std::string& key) {
    // Create GET command in RESP format
    auto array = protocol_.CreateArray();
    array->AddElement(std::move(protocol_.CreateBulkString("GET")));
    array->AddElement(std::move(protocol_.CreateBulkString(key)));

    std::string command = protocol_.Serialize(*array);

    auto response = SendCommand(command);
    if (!response) {
        return std::nullopt;
    }

    // Check if response is a bulk string
    if (response->type == RESPType::BULK_STRING) {
        const auto& bulk_string = static_cast<const BulkString&>(*response);
        return bulk_string.value;
    }

    // Check if response is a null bulk string (key not found)
    if (response->type == RESPType::BULK_STRING) {
        const auto& bulk_string = static_cast<const BulkString&>(*response);
        if (!bulk_string.value.has_value()) {
            return std::nullopt;
        }
    }

    return std::nullopt;
}

bool RedisClient::Delete(const std::string& key) {
    // Create DEL command in RESP format
    auto array = protocol_.CreateArray();
    array->AddElement(std::move(protocol_.CreateBulkString("DEL")));
    array->AddElement(std::move(protocol_.CreateBulkString(key)));

    std::string command = protocol_.Serialize(*array);

    auto response = SendCommand(command);
    if (!response) {
        return false;
    }

    // Check if response is an integer
    if (response->type == RESPType::INTEGER) {
        const auto& integer = static_cast<const Integer&>(*response);
        return integer.value > 0;
    }

    return false;
}

bool RedisClient::Ping() {
    // Create PING command in RESP format
    auto array = protocol_.CreateArray();
    array->AddElement(std::move(protocol_.CreateBulkString("PING")));

    std::string command = protocol_.Serialize(*array);

    auto response = SendCommand(command);
    if (!response) {
        return false;
    }

    // Check if response is a simple string with value "PONG"
    if (response->type == RESPType::SIMPLE_STRING) {
        const auto& simple_string = static_cast<const SimpleString&>(*response);
        return simple_string.value == "PONG";
    }

    return false;
}

bool RedisClient::SendData(const std::string& data) {
    ssize_t total_bytes_sent = 0;
    size_t total_bytes_to_send = data.size();

    while (total_bytes_sent < total_bytes_to_send) {
        ssize_t bytes_sent = send(client_socket_, data.c_str() + total_bytes_sent, 
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

std::string RedisClient::ReceiveData() {
    // For simplicity, we'll read up to 1024 bytes
    // A full implementation would need to handle variable-length responses
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket_, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        if (bytes_received < 0) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
        }
        return "";
    }

    buffer[bytes_received] = '\0';
    return std::string(buffer, bytes_received);
}

} // namespace redis
