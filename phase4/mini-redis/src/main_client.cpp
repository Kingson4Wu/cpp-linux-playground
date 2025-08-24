/**
 * @file main_client.cpp
 * @brief Main entry point for the mini-Redis client.
 *
 * This application implements a simple Redis-compatible client
 * that can connect to a Redis server and send commands.
 *
 * How to Compile and Run:
 *   1. From the project root directory:
 *      cmake -S . -B build
 *      cmake --build build -- -j
 *   2. Run the client:
 *      ./build/phase4/mini-redis/redis_client [HOST] [PORT]
 *   3. Or run with a single command:
 *      ./build/phase4/mini-redis/redis_client [HOST] [PORT] [COMMAND] [ARGS...]
 *
 * Usage:
 *   ./build/phase4/mini-redis/redis_client [HOST] [PORT]                    # Interactive mode
 *   ./build/phase4/mini-redis/redis_client [HOST] [PORT] [COMMAND] [ARGS...] # Single command mode
 *
 * Examples:
 *   # Run in interactive mode
 *   ./build/phase4/mini-redis/redis_client 127.0.0.1 6379
 *
 *   # Run a single command
 *   ./build/phase4/mini-redis/redis_client 127.0.0.1 6379 PING
 *
 * Supported Commands in Interactive Mode:
 *   PING                    - Ping the server
 *   SET key value           - Set a key-value pair
 *   GET key                 - Retrieve the value for a key
 *   DEL key                 - Delete a key
 *   EXISTS key              - Check if a key exists
 *   QUIT                    - Quit the client
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "redis_client") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "redis_client.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>

// Split a string by whitespace
std::vector<std::string> Split(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

// Execute a single command
bool ExecuteCommand(redis::RedisClient& client, const std::vector<std::string>& args) {
    if (args.empty()) {
        return true; // Empty command, just continue
    }

    const std::string& command = args[0];

    if (command == "PING") {
        bool result = client.Ping();
        if (result) {
            std::cout << "PONG" << std::endl;
        } else {
            std::cerr << "PING failed." << std::endl;
        }
        return result;
    } else if (command == "SET" && args.size() == 3) {
        const std::string& key = args[1];
        const std::string& value = args[2];
        bool result = client.Set(key, value);
        if (result) {
            std::cout << "OK" << std::endl;
        } else {
            std::cerr << "SET failed." << std::endl;
        }
        return result;
    } else if (command == "GET" && args.size() == 2) {
        const std::string& key = args[1];
        auto result = client.Get(key);
        if (result.has_value()) {
            std::cout << "\"" << result.value() << "\"" << std::endl;
        } else {
            std::cout << "(nil)" << std::endl;
        }
        return true; // This is still a successful operation, just no value found
    } else if (command == "DEL" && args.size() == 2) {
        const std::string& key = args[1];
        bool result = client.Delete(key);
        if (result) {
            std::cout << "(integer) 1" << std::endl;
        } else {
            std::cout << "(integer) 0" << std::endl;
        }
        return true; // This is still a successful operation, just no key deleted
    } else if (command == "EXISTS" && args.size() == 2) {
        const std::string& key = args[1];
        auto result = client.Get(key); // We're using Get to check existence for simplicity
        if (result.has_value()) {
            std::cout << "(integer) 1" << std::endl;
        } else {
            std::cout << "(integer) 0" << std::endl;
        }
        return true; // This is still a successful operation, just no key found
    } else if (command == "QUIT") {
        std::cout << "Goodbye!" << std::endl;
        return false; // Signal to quit
    } else {
        std::cerr << "Invalid command or arguments." << std::endl;
        std::cerr << "Supported commands: PING, SET, GET, DEL, EXISTS, QUIT" << std::endl;
        return true; // Continue even with invalid command
    }
}

int main(int argc, char* argv[]) {
    // Parse command line arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [HOST] [PORT] [COMMAND] [ARGS...]" << std::endl;
        std::cerr << "Examples:" << std::endl;
        std::cerr << "  " << argv[0] << " 127.0.0.1 6379" << std::endl;
        std::cerr << "  " << argv[0] << " 127.0.0.1 6379 PING" << std::endl;
        return 1;
    }

    std::string host = argv[1];

    int port;
    try {
        port = std::stoi(argv[2]);
        if (port <= 0 || port > 65535) {
            std::cerr << "Invalid port number. Must be between 1 and 65535." << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Invalid port number: " << argv[2] << std::endl;
        return 1;
    }

    // Create the client
    redis::RedisClient client;

    // Connect to the server
    if (!client.Connect(host, port)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    // If command arguments are provided, execute single command mode
    if (argc >= 4) {
        std::vector<std::string> args;
        for (int i = 3; i < argc; ++i) {
            args.push_back(argv[i]);
        }

        bool success = ExecuteCommand(client, args);
        client.Disconnect();
        std::cout << "Disconnected from Redis server." << std::endl;
        return success ? 0 : 1;
    }

    // Otherwise, run in interactive mode
    std::cout << "Connected to Redis server at " << host << ":" << port << std::endl;
    std::cout << "Type 'QUIT' to exit." << std::endl;

    std::string line;
    while (std::cout << "redis> " && std::getline(std::cin, line)) {
        // Parse and execute the command
        std::vector<std::string> args = Split(line);

        // Execute the command
        bool continue_loop = ExecuteCommand(client, args);
        if (!continue_loop) {
            break; // Quit command was executed
        }
    }

    // Disconnect from the server
    client.Disconnect();
    std::cout << "Disconnected from Redis server." << std::endl;
    return 0;
}