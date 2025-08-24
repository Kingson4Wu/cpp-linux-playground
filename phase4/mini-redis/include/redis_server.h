#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H

#include "kv_store.h"
#include "protocol.h"
#include <string>
#include <thread>
#include <vector>
#include <atomic>
#include <sys/socket.h>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace redis {

// Simple thread pool implementation
class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    // Delete copy constructor and assignment operator
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    void Enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    bool stop_;
};

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
     * @param num_threads The number of threads in the thread pool
     * @param timeout_seconds The timeout for network operations in seconds
     */
    explicit RedisServer(int port = 6379, size_t num_threads = 4, int timeout_seconds = 30);

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
    ThreadPool thread_pool_;
    int timeout_seconds_;

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
