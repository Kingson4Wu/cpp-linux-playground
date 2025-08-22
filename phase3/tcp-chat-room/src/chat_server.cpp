#include "chat_server.h"
#include "chat_session.h" // Include ChatSession header
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <algorithm>
#include <memory>
#include <chrono>

namespace chat_room {

ChatServer::ChatServer(int port, size_t num_threads)
    : port_(port), server_socket_(-1), running_(false), 
      thread_pool_(num_threads), next_client_id_(0) {
    // Initialize the server socket
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
        throw std::runtime_error("Failed to create server socket");
    }

    // Allow reuse of address and port
    int opt = 1;
    if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        close(server_socket_);
        throw std::runtime_error("Failed to set socket options");
    }

    // Bind the socket to the specified port
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket_, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        close(server_socket_);
        throw std::runtime_error("Failed to bind server socket");
    }

    // Listen for incoming connections
    if (listen(server_socket_, SOMAXCONN) == -1) {
        close(server_socket_);
        throw std::runtime_error("Failed to listen on server socket");
    }

    std::cout << "Chat server initialized on port " << port_ << std::endl;
}

ChatServer::~ChatServer() {
    if (server_socket_ != -1) {
        close(server_socket_);
    }
}

void ChatServer::Run() {
    std::cout << "Starting chat server..." << std::endl;
    running_ = true;

    // Start accepting connections
    AcceptConnections();

    std::cout << "Chat server stopped." << std::endl;
}

void ChatServer::Stop() {
    std::cout << "Stopping chat server..." << std::endl;
    running_ = false;

    // Close the server socket to interrupt accept()
    if (server_socket_ != -1) {
        close(server_socket_);
        server_socket_ = -1;
    }

    // Disconnect all clients
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        for (auto& pair : clients_) {
            pair.second->Stop();
        }
        clients_.clear();
    }
}

void ChatServer::AcceptConnections() {
    while (running_) {
        // Accept a new client connection
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket_, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);

        if (client_socket == -1) {
            if (running_) {
                std::cerr << "Failed to accept client connection" << std::endl;
            }
            // If running_ is false, accept() was interrupted by closing the socket
            // This is expected during shutdown
            continue;
        }

        std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) 
                  << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Enqueue the client handling task to the thread pool
        thread_pool_.Enqueue([this, client_socket]() {
            HandleClient(client_socket);
        });
    }
}

void ChatServer::HandleClient(int client_socket) {
    // Create a new ChatSession for the client
    int client_id = next_client_id_++;
    auto session = std::make_shared<ChatSession>(client_id, client_socket);

    // Add the session to the list of clients
    AddClient(session);

    // Start the session
    session->Start();

    // Wait for the session to finish (e.g., client disconnects)
    // In a real implementation, you might want to handle this differently
    // For example, you could have a separate cleanup thread or use a timeout

    // Remove the session from the list of clients
    RemoveClient(client_id);

    std::cout << "Client " << client_id << " disconnected" << std::endl;
}

void ChatServer::AddClient(std::shared_ptr<chat_room::ChatSession> session) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    clients_[session->GetId()] = session;
}

void ChatServer::RemoveClient(int client_id) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    clients_.erase(client_id);
}

void ChatServer::BroadcastMessage(const std::string& message, int sender_id) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    for (auto& pair : clients_) {
        if (pair.first != sender_id) {
            // Send the message to the client
            // This is a simplification. In a real implementation,
            // you would need to handle potential send errors
            // and possibly remove disconnected clients.
            pair.second->Send(message);
        }
    }
}

} // namespace chat_room
