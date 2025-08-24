#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H

#include "kv_store.h"
#include "protocol.h"
#include <string>
#include <thread>
#include <vector>
#include <atomic>
#include <sys/socket.h>

namespace redis {

/**
 * @brief A simple Redis-compatible server
 * 
 * This class implements a basic Redis server that can handle
 * client connections and execute Redis commands.
 */
class RedisServer {
public:
    /**
     * @brief Construct a new Redis Server object
     * @param port The port to listen on
     */
    explicit RedisServer(int port = 6379);

    /**
     * @brief Destroy the Redis Server object
     */
    ~RedisServer();

    // Delete copy constructor and assignment operator
    RedisServer(const RedisServer&) = delete;
    RedisServer& operator=(const RedisServer&) = delete;

    /**
     * @brief Start the server
     * @return true if the server started successfully, false otherwise
     */
    bool Start();

    /**
     * @brief Stop the server
     */
    void Stop();

private:
    int port_;
    std::atomic<bool> stop_;
    int server_socket_;
    KVStore store_;
    Protocol protocol_;

    /**
     * @brief Handle a client connection
     * @param client_socket The client socket
     */
    void HandleClient(int client_socket);

    /**
     * @brief Process a command from a client
     * @param client_socket The client socket
     */
    void ProcessCommand(int client_socket);

    /**
     * @brief Execute a Redis command
     * @param command The parsed command
     * @return A unique pointer to the response
     */
    std::unique_ptr<RESPValue> ExecuteCommand(const Array& command);

    /**
     * @brief Send a response to a client
     * @param client_socket The client socket
     * @param response The response to send
     * @return true if the response was sent successfully, false otherwise
     */
    bool SendResponse(int client_socket, const RESPValue& response);
};

} // namespace redis

#endif // REDIS_SERVER_H
