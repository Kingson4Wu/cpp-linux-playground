#include "chat_client.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <algorithm>
#include <chrono>

namespace chat_room {

ChatClient::ChatClient(const std::string& server_ip, int server_port)
    : server_ip_(server_ip), server_port_(server_port), 
      client_socket_(-1), connected_(false), running_(false) {
    // Constructor implementation
}

ChatClient::~ChatClient() {
    Disconnect();
}

bool ChatClient::Connect() {
    // Create a socket
    client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_ == -1) {
        std::cerr << "Failed to create client socket" << std::endl;
        return false;
    }

    // Set up the server address
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port_);

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, server_ip_.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid server IP address" << std::endl;
        close(client_socket_);
        client_socket_ = -1;
        return false;
    }

    // Connect to the server
    if (connect(client_socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        std::cerr << "Failed to connect to server" << std::endl;
        close(client_socket_);
        client_socket_ = -1;
        return false;
    }

    connected_ = true;
    std::cout << "Connected to server " << server_ip_ << ":" << server_port_ << std::endl;
    return true;
}

void ChatClient::Disconnect() {
    if (connected_) {
        std::cout << "Disconnecting from server..." << std::endl;
        connected_ = false;
        running_ = false;

        // Close the client socket
        if (client_socket_ != -1) {
            close(client_socket_);
            client_socket_ = -1;
        }

        // Wait for the receive and input threads to finish
        if (receive_thread_.joinable()) {
            receive_thread_.join();
        }
        if (input_thread_.joinable()) {
            input_thread_.join();
        }

        std::cout << "Disconnected from server" << std::endl;
    }
}

void ChatClient::Run() {
    if (!connected_) {
        std::cerr << "Not connected to server" << std::endl;
        return;
    }

    std::cout << "Starting chat client..." << std::endl;
    running_ = true;

    // Start the receive thread
    receive_thread_ = std::thread(&ChatClient::ReceiveMessages, this);

    // Start the input thread
    input_thread_ = std::thread(&ChatClient::ReadInput, this);

    // Wait for the threads to finish
    if (receive_thread_.joinable()) {
        receive_thread_.join();
    }
    if (input_thread_.joinable()) {
        input_thread_.join();
    }

    std::cout << "Chat client stopped" << std::endl;
}

void ChatClient::ReceiveMessages() {
    char buffer[1024];
    ssize_t bytes_read;

    while (running_) {
        bytes_read = recv(client_socket_, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            // Server disconnected or error
            if (bytes_read == 0) {
                std::cout << "Server disconnected" << std::endl;
            } else {
                std::cerr << "Error receiving data from server" << std::endl;
            }
            running_ = false;
            break;
        }

        buffer[bytes_read] = '\0'; // Null-terminate the received data

        // Display the received message
        std::cout << buffer << std::flush;
    }
}

void ChatClient::ReadInput() {
    std::string input_line;

    // Prompt for nickname
    std::cout << "Enter your nickname: ";
    std::getline(std::cin, nickname_);
    if (!nickname_.empty()) {
        SendMessage("/nick " + nickname_);
    }

    while (running_) {
        std::cout << "> ";
        std::getline(std::cin, input_line);

        if (!running_) {
            break;
        }

        if (input_line.empty()) {
            continue;
        }

        if (input_line == "/quit") {
            SendMessage(input_line);
            running_ = false;
            break;
        }

        SendMessage(input_line);
    }
}

void ChatClient::SendMessage(const std::string& message) {
    if (!connected_ || !running_) {
        return;
    }

    ssize_t bytes_sent = send(client_socket_, message.c_str(), message.length(), 0);
    if (bytes_sent == -1) {
        std::cerr << "Failed to send message to server" << std::endl;
        // Handle error (e.g., disconnect)
    } else if (static_cast<size_t>(bytes_sent) != message.length()) {
        std::cerr << "Partial send to server" << std::endl;
        // Handle partial send (e.g., retry or disconnect)
    }
}

} // namespace chat_room
