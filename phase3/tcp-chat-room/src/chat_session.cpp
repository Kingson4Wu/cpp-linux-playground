#include "chat_session.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <system_error>

namespace chat_room {

ChatSession::ChatSession(int id, int socket)
    : id_(id), socket_(socket), nickname_("User" + std::to_string(id)), running_(false) {
    // Constructor implementation
}

ChatSession::~ChatSession() {
    // Ensure the thread is joined before destroying the object
    if (receive_thread_.joinable()) {
        // If the thread is still running, we need to stop it
        // However, since this is the destructor, we can't call Stop() 
        // because it's a virtual method and the object might be partially destroyed
        // Instead, we'll just join the thread
        // This assumes that the thread will eventually exit on its own
        // In a real implementation, you would have a more robust mechanism
        // to signal the thread to stop before destruction
        receive_thread_.join();
    }

    if (socket_ != -1) {
        close(socket_);
    }
}

void ChatSession::Start() {
    std::cout << "Starting session for client " << id_ << " (" << nickname_ << ")" << std::endl;
    running_ = true;

    try {
        // Start the receive thread
        receive_thread_ = std::thread(&ChatSession::ReceiveMessages, this);
    } catch (const std::system_error& e) {
        std::cerr << "Failed to start receive thread for client " << id_ << ": " << e.what() << std::endl;
        running_ = false;
    }
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

    ssize_t bytes_sent = send(socket_, message.c_str(), message.length(), MSG_NOSIGNAL);
    if (bytes_sent == -1) {
        std::cerr << "Failed to send message to client " << id_ << std::endl;
        // Print the specific error
        perror("send");
        // Mark client as disconnected
        running_ = false;
    } else if (static_cast<size_t>(bytes_sent) != message.length()) {
        std::cerr << "Partial send to client " << id_ << std::endl;
        // Handle partial send (e.g., retry or mark client as disconnected)
        running_ = false;
    }
}

void ChatSession::ReceiveMessages() {
    char buffer[1024];
    ssize_t bytes_read;

    // Send a welcome message after a short delay to ensure client is ready
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
                // Print the specific error
                perror("recv");
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
