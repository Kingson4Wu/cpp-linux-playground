#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "thread_pool.h"
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <unordered_map>

// Forward declarations
struct ClientInfo;
namespace chat_room {
class ChatSession;
}

namespace chat_room {

/**
 * @brief A multi-threaded TCP chat server.
 * 
 * This server listens for incoming client connections and handles
 * each client in a separate thread from a thread pool. It broadcasts
 * messages from one client to all other connected clients.
 */
class ChatServer {
public:
    /**
     * @brief Constructs a ChatServer.
     * 
     * @param port The port number to listen on.
     * @param num_threads The number of threads in the thread pool.
     */
    ChatServer(int port, size_t num_threads = 4);

    /**
     * @brief Destructor. Stops the server and cleans up resources.
     */
    ~ChatServer();

    /**
     * @brief Starts the server and begins listening for connections.
     * 
     * This method will block until the server is stopped.
     */
    void Run();

    /**
     * @brief Stops the server.
     * 
     * This method signals the server to stop accepting new connections
     * and closes all existing client connections.
     */
    void Stop();

    // Methods for ChatSession to interact with the server
    void AddClient(std::shared_ptr<ChatSession> session);
    void RemoveClient(int client_id);
    void BroadcastMessage(const std::string& message, int sender_id = -1);

private:
    int port_;                          ///< Port number to listen on
    int server_socket_;                  ///< Server socket file descriptor
    bool running_;                      ///< Flag to indicate if the server is running

    ThreadPool thread_pool_;            ///< Thread pool for handling client connections

    // Client management
    std::unordered_map<int, std::shared_ptr<ChatSession>> clients_; ///< Connected clients
    std::mutex clients_mutex_;          ///< Mutex to protect the clients map
    int next_client_id_;                ///< ID counter for new clients

    /**
     * @brief Accepts incoming client connections.
     * 
     * This method runs in a loop, accepting new client connections
     * and adding them to the thread pool for handling.
     */
    void AcceptConnections();

    /**
     * @brief Handles a single client connection.
     * 
     * This method is executed by a worker thread from the thread pool.
     * It performs the initial handshake with the client and then
     * creates a ChatSession to manage the client's connection.
     * 
     * @param client_socket The client's socket file descriptor.
     */
    void HandleClient(int client_socket);
};

} // namespace chat_room

#endif // CHAT_SERVER_H
