#include "redis_server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <thread>
#include <vector>

namespace redis {

RedisServer::RedisServer(int port) 
    : port_(port), stop_(false), server_socket_(-1) {}

RedisServer::~RedisServer() {
    Stop();
}

bool RedisServer::Start() {
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

    std::cout << "Redis server listening on port " << port_ << std::endl;

    // Main server loop
    while (!stop_) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        // Accept a new connection
        int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            if (!stop_) {
                std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
            }
            continue;
        }

        std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) 
                  << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Handle the connection in a separate thread
        std::thread client_thread([this, client_socket]() {
            HandleClient(client_socket);
        });
        client_thread.detach();
    }

    std::cout << "Server has stopped." << std::endl;
    return true;
}

void RedisServer::Stop() {
    if (!stop_) {
        stop_ = true;

        // Close the server socket to unblock accept()
        if (server_socket_ >= 0) {
            close(server_socket_);
            server_socket_ = -1;
        }
    }
}

void RedisServer::HandleClient(int client_socket) {
    // Process commands from the client
    ProcessCommand(client_socket);

    // Close the client socket
    close(client_socket);
    std::cout << "Client connection closed." << std::endl;
}

void RedisServer::ProcessCommand(int client_socket) {
    // For simplicity, we'll read line by line in this implementation
    // A full implementation would need to handle the complete RESP protocol

    char buffer[1024];
    while (!stop_) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            if (bytes_received < 0) {
                std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            }
            break;
        }

        buffer[bytes_received] = '\0';
        std::string data(buffer);

        // Parse the RESP message
        auto resp_value = protocol_.Parse(data);
        if (!resp_value) {
            // Send error response
            auto error = protocol_.CreateError("ERR Invalid command format");
            std::string response = protocol_.Serialize(*error);
            send(client_socket, response.c_str(), response.size(), 0);
            continue;
        }

        // Check if it's an array (command)
        if (resp_value->type == RESPType::ARRAY) {
            const auto& command = static_cast<const Array&>(*resp_value);
            auto response = ExecuteCommand(command);
            if (response) {
                std::string response_str = protocol_.Serialize(*response);
                send(client_socket, response_str.c_str(), response_str.size(), 0);
            }
        } else {
            // Send error response
            auto error = protocol_.CreateError("ERR Invalid command format");
            std::string response = protocol_.Serialize(*error);
            send(client_socket, response.c_str(), response.size(), 0);
        }
    }
}

std::unique_ptr<RESPValue> RedisServer::ExecuteCommand(const Array& command) {
    // Check if we have at least one element (the command name)
    if (command.elements.empty()) {
        return protocol_.CreateError("ERR Empty command");
    }

    // Get the command name (first element should be a bulk string)
    if (command.elements[0]->type != RESPType::BULK_STRING) {
        return protocol_.CreateError("ERR Invalid command format");
    }

    const auto& cmd_name_bulk = static_cast<const BulkString&>(*command.elements[0]);
    if (!cmd_name_bulk.value.has_value()) {
        return protocol_.CreateError("ERR Invalid command name");
    }

    std::string cmd_name = cmd_name_bulk.value.value();

    // Convert to uppercase for case-insensitive comparison
    for (auto& c : cmd_name) {
        c = std::toupper(c);
    }

    // Handle different commands
    if (cmd_name == "PING") {
        return protocol_.CreateSimpleString("PONG");
    } else if (cmd_name == "SET" && command.elements.size() == 3) {
        // SET key value
        if (command.elements[1]->type != RESPType::BULK_STRING ||
            command.elements[2]->type != RESPType::BULK_STRING) {
            return protocol_.CreateError("ERR Invalid SET command format");
        }

        const auto& key_bulk = static_cast<const BulkString&>(*command.elements[1]);
        const auto& value_bulk = static_cast<const BulkString&>(*command.elements[2]);

        if (!key_bulk.value.has_value() || !value_bulk.value.has_value()) {
            return protocol_.CreateError("ERR Invalid key or value");
        }

        store_.Set(key_bulk.value.value(), value_bulk.value.value());
        return protocol_.CreateSimpleString("OK");
    } else if (cmd_name == "GET" && command.elements.size() == 2) {
        // GET key
        if (command.elements[1]->type != RESPType::BULK_STRING) {
            return protocol_.CreateError("ERR Invalid GET command format");
        }

        const auto& key_bulk = static_cast<const BulkString&>(*command.elements[1]);

        if (!key_bulk.value.has_value()) {
            return protocol_.CreateError("ERR Invalid key");
        }

        auto value = store_.Get(key_bulk.value.value());
        if (value.has_value()) {
            return protocol_.CreateBulkString(value.value());
        } else {
            return protocol_.CreateNullBulkString();
        }
    } else if (cmd_name == "DEL" && command.elements.size() == 2) {
        // DEL key
        if (command.elements[1]->type != RESPType::BULK_STRING) {
            return protocol_.CreateError("ERR Invalid DEL command format");
        }

        const auto& key_bulk = static_cast<const BulkString&>(*command.elements[1]);

        if (!key_bulk.value.has_value()) {
            return protocol_.CreateError("ERR Invalid key");
        }

        bool deleted = store_.Delete(key_bulk.value.value());
        return protocol_.CreateInteger(deleted ? 1 : 0);
    } else if (cmd_name == "EXISTS" && command.elements.size() == 2) {
        // EXISTS key
        if (command.elements[1]->type != RESPType::BULK_STRING) {
            return protocol_.CreateError("ERR Invalid EXISTS command format");
        }

        const auto& key_bulk = static_cast<const BulkString&>(*command.elements[1]);

        if (!key_bulk.value.has_value()) {
            return protocol_.CreateError("ERR Invalid key");
        }

        bool exists = store_.Exists(key_bulk.value.value());
        return protocol_.CreateInteger(exists ? 1 : 0);
    } else {
        return protocol_.CreateError("ERR Unknown command '" + cmd_name_bulk.value.value() + "'");
    }
}

bool RedisServer::SendResponse(int client_socket, const RESPValue& response) {
    std::string response_str = protocol_.Serialize(response);
    ssize_t bytes_sent = send(client_socket, response_str.c_str(), response_str.size(), 0);
    return bytes_sent == static_cast<ssize_t>(response_str.size());
}

} // namespace redis
