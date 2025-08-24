#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include "protocol.h"
#include <string>
#include <sys/socket.h>
#include <optional>

namespace redis {

/**
 * @brief A simple Redis-compatible client
 * 
 * This class implements a basic Redis client that can connect
 * to a Redis server and send commands. The client maintains
 * a persistent connection to the server until explicitly disconnected.
 */
class RedisClient {
public:
    /**
     * @brief Construct a new Redis Client object
     * @param timeout_seconds The timeout for network operations in seconds
     */
    RedisClient(int timeout_seconds = 30);

    /**
     * @brief Destroy the Redis Client object
     */
    ~RedisClient();

    // Delete copy constructor and assignment operator
    RedisClient(const RedisClient&) = delete;
    RedisClient& operator=(const RedisClient&) = delete;

    /**
     * @brief Connect to a Redis server
     * @param host The server host
     * @param port The server port
     * @return true if the connection was successful, false otherwise
     */
    bool Connect(const std::string& host, int port);

    /**
     * @brief Disconnect from the Redis server
     */
    void Disconnect();

    /**
     * @brief Send a command to the server
     * @param command The command to send
     * @return A unique pointer to the response, or nullptr on error
     */
    std::unique_ptr<RESPValue> SendCommand(const std::string& command);

    /**
     * @brief Send a SET command
     * @param key The key to set
     * @param value The value to set
     * @return true if the operation was successful, false otherwise
     */
    bool Set(const std::string& key, const std::string& value);

    /**
     * @brief Send a GET command
     * @param key The key to get
     * @return The value associated with the key, or std::nullopt if the key doesn't exist
     */
    std::optional<std::string> Get(const std::string& key);

    /**
     * @brief Send a DEL command
     * @param key The key to delete
     * @return true if the key existed and was deleted, false if the key didn't exist
     */
    bool Delete(const std::string& key);

    /**
     * @brief Send a PING command
     * @return true if the server responded with PONG, false otherwise
     */
    bool Ping();

    /**
     * @brief Send an EXISTS command
     * @param key The key to check
     * @return true if the key exists, false otherwise
     */
    bool Exists(const std::string& key);

private:
    int client_socket_;
    Protocol protocol_;
    int timeout_seconds_;

    /**
     * @brief Send raw data to the server
     * @param data The data to send
     * @return true if the data was sent successfully, false otherwise
     */
    bool SendData(const std::string& data);

    /**
     * @brief Receive data from the server
     * @return The received data, or an empty string on error
     */
    std::string ReceiveData();
};

} // namespace redis

#endif // REDIS_CLIENT_H
