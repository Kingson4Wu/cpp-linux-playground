#include "chat_session.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sstream>
#include <algorithm>
#include <cstring>

namespace chat_room {

ChatSession::ChatSession(int id, int socket)
    : id_(id), socket_(socket), nickname_("User" + std::to_string(id)), running_(false) {
    // Constructor implementation
}

ChatSession::~ChatSession() {
    if (socket_ != -1) {
        close(socket_);
    }
}

void ChatSession::Start() {
    std::cout << "Starting session for client " << id_ << " (" << nickname_ << ")" << std::endl;
    running_ = true;

    // Start the receive thread
    receive_thread_ = std::thread(&ChatSession::ReceiveMessages, this);
}

void ChatSession::Stop() {
    std::cout << "Stopping session for client " << id_ << " (" << nickname_ << ")" << std::endl;
    running_ = false;

    // Wait for the receive thread to finish
    if (receive_thread_.joinable()) {
        receive_thread_.join();
    }
}

void ChatSession::Send(const std::string& message) {
    if (!running_) {
        return;
    }

    ssize_t bytes_sent = send(socket_, message.c_str(), message.length(), 0);
    if (bytes_sent == -1) {
        std::cerr << "Failed to send message to client " << id_ << std::endl;
        // Handle error (e.g., mark client as disconnected)
    } else if (static_cast<size_t>(bytes_sent) != message.length()) {
        std::cerr << "Partial send to client " << id_ << std::endl;
        // Handle partial send (e.g., retry or mark client as disconnected)
    }
}

void ChatSession::ReceiveMessages() {
    char buffer[1024];
    ssize_t bytes_read;

    // Send a welcome message
    std::string welcome_msg = "Welcome, " + nickname_ + "! Type /help for a list of commands.
";
    Send(welcome_msg);

    while (running_) {
        bytes_read = recv(socket_, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            // Client disconnected or error
            if (bytes_read == 0) {
                std::cout << "Client " << id_ << " (" << nickname_ << ") disconnected" << std::endl;
            } else {
                std::cerr << "Error receiving data from client " << id_ << std::endl;
            }
            break;
        }

        buffer[bytes_read] = '\0'; // Null-terminate the received data

        // Process the received message
        std::string message(buffer);

        // Remove trailing newline characters
        message.erase(std::remove(message.begin(), message.end(), '
'), message.end());
        message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());

        std::cout << "Received from client " << id_ << " (" << nickname_ << "): " << message << std::endl;

        // Process commands
        if (message == "/quit") {
            Send("Goodbye!
");
            break; // This will cause the loop to exit and the session to stop
        } else if (message.substr(0, 5) == "/nick") {
            std::istringstream iss(message);
            std::string command, new_nickname;
            iss >> command >> new_nickname;
            if (!new_nickname.empty()) {
                std::string old_nickname = nickname_;
                nickname_ = new_nickname;
                Send("Nickname changed from " + old_nickname + " to " + nickname_ + "
");
            } else {
                Send("Usage: /nick <new_nickname>
");
            }
        } else if (message == "/help") {
            Send("Available commands:
/nick <new_nickname> - Change your nickname
/quit - Disconnect from the server
/help - Show this help message
");
        } else {
            // Broadcast the message to all other clients
            // This would typically be done by calling a method on the ChatServer
            // For now, we'll just echo it back to the client
            Send(nickname_ + ": " + message + "
");
        }
    }

    running_ = false; // Ensure running_ is false
}

} // namespace chat_room
