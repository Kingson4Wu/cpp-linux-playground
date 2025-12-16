---
sidebar_position: 2
---

# TCP Multi-threaded Chat Room Implementation

Build a real-time, multi-threaded chat application that allows multiple users to connect and exchange messages simultaneously. This project introduces you to socket programming, concurrent client handling, and real-time communication protocols.

## Project Structure

The chat room is organized as follows:

```
phase3/tcp-chat-room/
├── CMakeLists.txt
├── include/
│   └── chat_server.h  # Chat server interface and classes
└── src/
    └── chat_server.cpp  # Implementation of chat server functionality
```

## Learning Objectives

During this project, you will:

- Implement TCP server socket programming
- Handle multiple concurrent client connections using threading
- Design a simple text-based communication protocol
- Manage shared state across multiple threads safely
- Implement broadcast messaging to all connected clients
- Apply proper error handling and resource cleanup for network connections

## Core Concepts

### Socket Programming Fundamentals

Understand these key socket operations:

- **Socket Creation**: `socket()` system call to create a communication endpoint
- **Binding**: `bind()` to associate the socket with an address and port
- **Listening**: `listen()` to prepare the socket to accept connections
- **Accepting**: `accept()` to accept incoming client connections
- **Sending/Receiving**: `send()`/`recv()` for data transmission
- **Closing**: `close()` to release socket resources

### Client-Server Architecture

The chat server follows a multi-threaded model:

1. Main server thread accepts new client connections
2. Each client gets its own thread for communication
3. Clients can send messages that are broadcast to all other clients
4. Server maintains a list of connected clients

## Implementation Architecture

### ChatServer Interface

```cpp
class ChatServer {
public:
    explicit ChatServer(int port);
    void start();
    void stop();
    
private:
    int server_fd_;
    int port_;
    std::atomic<bool> running_;
    std::vector<std::thread> client_threads_;
    std::vector<int> client_fds_;
    std::mutex clients_mutex_;
    
    void accept_clients();
    void handle_client(int client_fd);
    void broadcast_message(const std::string& message, int sender_fd);
    void remove_client(int client_fd);
};
```

### Message Protocol

Design a simple text-based protocol for client-server communication:

- Messages terminated by newline characters (`\n`)
- Special commands prefixed with `/` (e.g., `/nick username` to change nickname)
- Server responses in the format `[SERVER] message`

## Implementation Requirements

### Core Features

1. **Multi-client Support**: Handle multiple concurrent client connections
2. **Message Broadcasting**: Forward messages from one client to all others
3. **Client Management**: Add/remove clients as they connect/disconnect
4. **Thread Safety**: Protect shared data structures with mutexes
5. **Graceful Shutdown**: Properly terminate all client threads and close connections

### Error Handling

Implement proper error handling for:

- Failed socket operations
- Connection drops by clients
- Network timeouts
- Invalid message formats
- Resource exhaustion

### Sample Implementation Approach

```cpp
class ChatServer {
private:
    struct ClientInfo {
        int fd;
        std::string nickname;
        std::thread receive_thread;
    };
    
    std::vector<ClientInfo> clients_;
    std::mutex clients_mutex_;
    
    void accept_loop();
    static void client_handler(ChatServer* server, int client_fd);
    void broadcast_to_all(const std::string& message, int sender_fd);
    void cleanup_disconnected_clients();
    
public:
    void start();
    void stop();
};
```

## Concurrency Considerations

### Thread Management

- Create a new thread for each connected client
- Use thread-safe collections for shared state
- Implement proper thread cleanup when clients disconnect
- Consider using thread pools for better resource management (advanced)

### Synchronization

- Protect the client list with a mutex
- Use atomic operations for simple shared state (like server running flag)
- Avoid deadlock by using consistent lock ordering

## Performance Considerations

- Minimize lock contention on the client list
- Use efficient string operations for message handling
- Consider using memory pools for message buffers
- Implement proper connection limits to prevent resource exhaustion

## Testing Strategy

Test your chat server with:

- Single client connection and messaging
- Multiple concurrent clients
- Message broadcasting to all clients
- Client disconnection handling
- Server shutdown with active clients
- Network error simulation (using tools like `netem`)

## Client Example

For testing, you can use a simple TCP client like `telnet` or `nc`:

```bash
# Connect to chat server on localhost port 8080
telnet localhost 8080

# Or using netcat
nc localhost 8080
```

## Usage Example

```cpp
#include "chat_server.h"
#include <iostream>

int main() {
    try {
        ChatServer server(8080);
        std::cout << "Chat server starting on port 8080..." << std::endl;
        
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

## Advanced Features (Optional)

Consider implementing additional capabilities:

1. **Private Messages**: `/msg username message` for direct communication
2. **User Names**: Allow users to set nicknames with `/nick username`
3. **Rooms/Channels**: Multiple chat rooms with `/join room_name`
4. **Message Persistence**: Log messages to file
5. **Rate Limiting**: Prevent spam by limiting message frequency

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# Start the chat server
./phase3/tcp-chat-room/chat_server

# Test with multiple telnet/nc connections to localhost:8080
```

## Next Steps

After completing the chat room, move on to the [HTTP Static File Server](./http-server.md) to explore web protocols.