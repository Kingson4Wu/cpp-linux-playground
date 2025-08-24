#include "redis_client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <sys/select.h>
#include <sys/time.h>

namespace redis {

RedisClient::RedisClient(int timeout_seconds) : client_socket_(-1), timeout_seconds_(timeout_seconds) {}

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
    // Use select for timeout
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_socket_, &read_fds);
    
    struct timeval timeout;
    timeout.tv_sec = timeout_seconds_;
    timeout.tv_usec = 0;
    
    int select_result = select(client_socket_ + 1, &read_fds, NULL, NULL, &timeout);
    if (select_result <= 0) {
        if (select_result < 0) {
            std::cerr << "Error in select: " << strerror(errno) << std::endl;
        } else {
            std::cerr << "Timeout while receiving data" << std::endl;
        }
        return "";
    }
    
    // First, read the first byte to determine the type of RESP value
    char first_byte;
    ssize_t bytes_received = recv(client_socket_, &first_byte, 1, 0);
    if (bytes_received <= 0) {
        if (bytes_received < 0) {
            std::cerr << "Error receiving first byte: " << strerror(errno) << std::endl;
        }
        return "";
    }
    
    std::string response;
    response += first_byte;
    
    // Based on the first byte, determine how much more data to read
    switch (first_byte) {
        case '+': // Simple string
        case '-': // Error
        case ':': // Integer
            // Read until we get \r\n
            while (true) {
                char c;
                bytes_received = recv(client_socket_, &c, 1, 0);
                if (bytes_received <= 0) {
                    if (bytes_received < 0) {
                        std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
                    }
                    return "";
                }
                
                response += c;
                
                // Check if we have \r\n at the end
                if (response.length() >= 2 && 
                    response[response.length()-2] == '\r' && 
                    response[response.length()-1] == '\n') {
                    break;
                }
            }
            break;
            
        case '$': // Bulk string
            {
                // Read the length line
                std::string length_line;
                bool found_end = false;
                while (!found_end) {
                    char c;
                    bytes_received = recv(client_socket_, &c, 1, 0);
                    if (bytes_received <= 0) {
                        if (bytes_received < 0) {
                            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
                        }
                        return "";
                    }
                    
                    length_line += c;
                    
                    // Check if we have \r\n at the end
                    if (length_line.length() >= 2 && 
                        length_line[length_line.length()-2] == '\r' && 
                        length_line[length_line.length()-1] == '\n') {
                        found_end = true;
                    }
                }
                
                response += length_line;
                
                // Parse the length
                try {
                    std::string length_str = length_line.substr(0, length_line.length()-2); // Remove \r\n
                    long long length = std::stoll(length_str);
                    
                    // Handle null bulk string
                    if (length == -1) {
                        // Nothing more to read
                        break;
                    }
                    
                    // Read the data plus \r\n
                    for (long long i = 0; i < length + 2; ++i) {
                        char c;
                        bytes_received = recv(client_socket_, &c, 1, 0);
                        if (bytes_received <= 0) {
                            if (bytes_received < 0) {
                                std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
                            }
                            return "";
                        }
                        
                        response += c;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing bulk string length: " << e.what() << std::endl;
                    return "";
                }
            }
            break;
            
        case '*': // Array
            {
                // Read the length line
                std::string length_line;
                bool found_end = false;
                while (!found_end) {
                    char c;
                    bytes_received = recv(client_socket_, &c, 1, 0);
                    if (bytes_received <= 0) {
                        if (bytes_received < 0) {
                            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
                        }
                        return "";
                    }
                    
                    length_line += c;
                    
                    // Check if we have \r\n at the end
                    if (length_line.length() >= 2 && 
                        length_line[length_line.length()-2] == '\r' && 
                        length_line[length_line.length()-1] == '\n') {
                        found_end = true;
                    }
                }
                
                response += length_line;
                
                // Parse the length
                try {
                    std::string length_str = length_line.substr(0, length_line.length()-2); // Remove \r\n
                    long long length = std::stoll(length_str);
                    
                    // Handle null array
                    if (length == -1) {
                        // Nothing more to read
                        break;
                    }
                    
                    // For each element in the array, recursively call ReceiveData
                    for (long long i = 0; i < length; ++i) {
                        std::string element = ReceiveData();
                        if (element.empty()) {
                            return "";
                        }
                        response += element;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing array length: " << e.what() << std::endl;
                    return "";
                }
            }
            break;
            
        default:
            std::cerr << "Unknown RESP type: " << first_byte << std::endl;
            return "";
    }
    
    return response;
}

} // namespace redis