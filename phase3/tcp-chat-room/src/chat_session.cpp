#include "chat_session.h"
#include "chat_server.h"  // Include chat_server.h to access ChatServer::GetInstance()
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
    // Ensure the threads are joined before destroying the object
    if (receive_thread_.joinable()) {
        receive_thread_.join();
    }
    
    if (send_thread_.joinable()) {
        send_thread_.join();
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
        
        // Start the send thread
        send_thread_ = std::thread(&ChatSession::SendMessages, this);
    } catch (const std::system_error& e) {
        std::cerr << "Failed to start thread for client " << id_ << ": " << e.what() << std::endl;
        running_ = false;
    }
}

void ChatSession::Stop() {
    std::cout << "Stopping session for client " << id_ << " (" << nickname_ << ")" << std::endl;
    running_ = false;

    // Notify the send thread to wake up in case it's waiting
    send_condition_.notify_all();

    // Wait for the receive thread to finish
    if (receive_thread_.joinable()) {
        receive_thread_.join();
    }
    
    // Wait for the send thread to finish
    if (send_thread_.joinable()) {
        send_thread_.join();
    }
}

void ChatSession::Send(const std::string& message) {
    if (!running_) {
        return;
    }

    // Add the message to the send queue
    {
        std::lock_guard<std::mutex> lock(send_queue_mutex_);
        send_queue_.push(message);
    }
    
    // Notify the send thread that a message is available
    send_condition_.notify_one();
}

void ChatSession::SendMessages() {
    while (running_) {
        std::string message;
        
        // Wait for a message to be available in the queue
        {
            std::unique_lock<std::mutex> lock(send_queue_mutex_);
            send_condition_.wait(lock, [this] { 
                return !send_queue_.empty() || !running_; 
            });
            
            // If the session is no longer running, exit the thread
            if (!running_ && send_queue_.empty()) {
                return;
            }
            
            // Get the next message from the queue
            if (!send_queue_.empty()) {
                message = send_queue_.front();
                send_queue_.pop();
            }
        }
        
        // If we have a message, send it
        if (!message.empty()) {
            ssize_t bytes_sent = send(socket_, message.c_str(), message.length(), MSG_NOSIGNAL);
            if (bytes_sent == -1) {
                std::cerr << "Failed to send message to client " << id_ << std::endl;
                // Print the specific error
                perror("send");
                // Mark client as disconnected
                running_ = false;
                break;
            } else if (static_cast<size_t>(bytes_sent) != message.length()) {
                std::cerr << "Partial send to client " << id_ << std::endl;
                // Handle partial send (e.g., retry or mark client as disconnected)
                running_ = false;
                break;
            }
        }
    }
}

void ChatSession::ReceiveMessages() {
    char buffer[1024];
    ssize_t bytes_read;

    // Send a welcome message after a short delay to ensure client is ready
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::string welcome_msg = "Welcome, " + nickname_ + "! Type /help for a list of commands.\n";
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
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
        message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());

        std::cout << "Received from client " << id_ << " (" << nickname_ << "): " << message << std::endl;

        // Process commands
        if (message == "/quit") {
            Send("Goodbye!\n");
            break; // This will cause the loop to exit and the session to stop
        } else if (message.substr(0, 5) == "/nick") {
            std::istringstream iss(message);
            std::string command, new_nickname;
            iss >> command >> new_nickname;
            if (!new_nickname.empty()) {
                std::string old_nickname = nickname_;
                nickname_ = new_nickname;
                Send("Nickname changed from " + old_nickname + " to " + nickname_ + "\n");
            } else {
                Send("Usage: /nick <new_nickname>\n");
            }
        } else if (message == "/help") {
            Send("Available commands:\n/nick <new_nickname> - Change your nickname\n/quit - Disconnect from the server\n/help - Show this help message\n");
        } else {
            // Broadcast the message to all other clients
            // Get the server instance and call its broadcast method
            auto server = ChatServer::GetInstance();
            if (server) {
                server->BroadcastMessage(nickname_ + ": " + message + "\n", id_);
            }
        }
    }

    running_ = false; // Ensure running_ is false
}

} // namespace chat_room