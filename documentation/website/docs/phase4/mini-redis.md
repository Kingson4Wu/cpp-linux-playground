---
sidebar_position: 2
---

# mini-Redis Implementation

Build a simplified Redis-compatible key-value store that supports common data types and operations. This project combines network programming with efficient data structures and memory management techniques.

## Project Structure

The mini-Redis is organized as follows:

```
phase4/mini-redis/
├── CMakeLists.txt
├── include/
│   └── redis_server.h  # Redis server interface and classes
└── src/
    └── redis_server.cpp  # Implementation of Redis functionality
```

## Learning Objectives

During this project, you will:

- Design an in-memory data store with multiple data type support
- Implement the Redis protocol (RESP - Redis Serialization Protocol)
- Create efficient data structures for key-value storage
- Build a network server that handles concurrent client requests
- Implement persistence mechanisms for data durability
- Apply performance optimization techniques for high-throughput operations
- Design command processing and validation systems

## Core Concepts

### Redis Protocol (RESP)

Redis uses a specific text-based protocol called RESP (Redis Serialization Protocol):

- **Simple Strings**: `+OK\r\n`
- **Errors**: `-Error message\r\n`
- **Integers**: `:123\r\n`
- **Bulk Strings**: `$5\r\nhello\r\n`
- **Arrays**: `*2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n`

### Data Types Support

Your mini-Redis should support:

- **Strings**: Simple key-value pairs
- **Hashes**: Field-value mappings within keys
- **Lists**: Ordered collections of strings
- **Sets**: Unordered collections of unique strings
- **Sorted Sets**: Collections with scores for ranking

## Implementation Architecture

### RedisServer Interface

```cpp
class RedisServer {
public:
    explicit RedisServer(int port, const std::string& data_dir = "");
    void start();
    void stop();
    
    // Core data operations
    std::string get(const std::string& key);
    bool set(const std::string& key, const std::string& value);
    bool del(const std::string& key);
    std::vector<std::string> keys(const std::string& pattern);
    
private:
    int server_fd_;
    int port_;
    std::string data_dir_;
    std::atomic<bool> running_;
    std::unordered_map<int, std::thread> client_threads_;
    
    // Data storage structures
    std::unordered_map<std::string, std::string> string_store_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> hash_store_;
    std::unordered_map<std::string, std::list<std::string>> list_store_;
    std::unordered_map<std::string, std::unordered_set<std::string>> set_store_;
    std::unordered_map<std::string, std::map<double, std::string>> zset_store_;
    
    // Synchronization
    std::shared_mutex store_mutex_;
    
    void accept_connections();
    void handle_client(int client_fd);
    std::string process_command(const std::vector<std::string>& args);
    std::string serialize_response(const std::string& response);
};
```

### Command Processing

Implement a command router to handle different Redis commands:

```cpp
class CommandProcessor {
public:
    static std::string execute(const std::vector<std::string>& args, RedisServer& server);
    
private:
    // Individual command handlers
    static std::string handle_get(const std::vector<std::string>& args, RedisServer& server);
    static std::string handle_set(const std::vector<std::string>& args, RedisServer& server);
    static std::string handle_del(const std::vector<std::string>& args, RedisServer& server);
    static std::string handle_keys(const std::vector<std::string>& args, RedisServer& server);
    static std::string handle_ping(const std::vector<std::string>& args, RedisServer& server);
};
```

## Implementation Requirements

### Core Features

1. **Network Interface**: TCP server that accepts Redis protocol commands
2. **Data Types**: Support for strings, hashes, lists, sets, and sorted sets
3. **Persistence**: Optional RDB-like snapshotting to disk
4. **Threading**: Handle concurrent client connections safely
5. **Memory Efficiency**: Use appropriate data structures to minimize memory usage
6. **Command Support**: Implement common Redis commands (GET, SET, DEL, KEYS, etc.)

### Supported Commands

At minimum, implement:
- `SET key value` - Set a key-value pair
- `GET key` - Get value of a key
- `DEL key` - Delete a key
- `KEYS pattern` - List keys matching pattern
- `PING` - Health check command
- `EXISTS key` - Check if key exists

### Sample Implementation Approach

```cpp
class RedisServer {
private:
    // Use specialized data structures for different types
    struct Value {
        std::string data;
        std::chrono::steady_clock::time_point expiry;
    };
    
    std::unordered_map<std::string, Value> store_;
    mutable std::shared_mutex store_mutex_;
    
    // Use shared_mutex for read-heavy workloads
    std::string get_impl(const std::string& key) const;
    bool set_impl(const std::string& key, const std::string& value, std::chrono::seconds ttl = {});
    
public:
    std::string get(const std::string& key);
    bool set(const std::string& key, const std::string& value);
    void save_snapshot(); // For persistence
};
```

## Performance Considerations

- **Lock Granularity**: Consider sharding the store to reduce lock contention
- **Memory Management**: Use memory pools for frequently allocated objects
- **Command Pipelining**: Process multiple commands in batch for better throughput
- **Connection Management**: Efficient handling of client connections
- **Expiring Keys**: Implement lazy or active expiration mechanisms

## Persistence Strategies

Consider implementing one of these persistence methods:

1. **RDB-like Snapshots**: Periodic snapshots of the entire dataset
2. **AOF (Append-Only File)**: Log all write operations for reconstruction
3. **Hybrid Approach**: Combine RDB snapshots with incremental logging

## Testing Strategy

Test your mini-Redis with:

- Individual command correctness
- Concurrent client operations
- Memory usage under load
- Persistence functionality (if implemented)
- Network protocol compliance
- Performance benchmarks against actual Redis commands

## Usage Example

```cpp
#include "redis_server.h"
#include <iostream>

int main() {
    try {
        RedisServer server(6379); // Standard Redis port
        std::cout << "Mini-Redis server starting on port 6379..." << std::endl;
        
        server.start();
        
        // Wait for user to stop the server
        std::cout << "Press Enter to stop the server..." << std::endl;
        std::cin.get();
        
        server.stop();
        std::cout << "Server stopped." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Testing with Clients

Test your server with Redis-compatible clients:

```bash
# With redis-cli (if available)
redis-cli -p 6379 SET test_key "Hello, World!"
redis-cli -p 6379 GET test_key

# With telnet or netcat (for protocol testing)
telnet localhost 6379
# Send: *3\r\n$3\r\nSET\r\n$8\r\ntest_key\r\n$12\r\nHello, World!\r\n
```

## Advanced Features (Optional)

Consider implementing additional capabilities:

1. **Transactions**: Multi-command atomic operations
2. **Pub/Sub**: Publish/subscribe messaging system
3. **Lua Scripting**: Server-side execution of Lua scripts
4. **Replication**: Master-slave data replication
5. **Memory Eviction**: LRU-based key expiration strategies

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# Start the mini-Redis server
./phase4/mini-redis/mini_redis_server

# Test with redis-cli or custom client at port 6379
```

## Next Steps

After completing mini-Redis, move on to the [mini-Search](./mini-search.md) project to build a search engine.