---
sidebar_position: 1
---

# Phase 3: Network Programming

Phase 3 focuses on network programming, where you'll build applications that communicate over networks using TCP/IP and HTTP protocols. This phase covers socket programming, concurrent network I/O, and building robust network services.

## Learning Objectives

By the end of Phase 3, you will:

- Master TCP/IP socket programming in C++
- Implement concurrent network servers using threading or event loops
- Build HTTP servers with proper request/response handling
- Handle network errors and implement robust connection management
- Apply security best practices for network applications
- Understand network protocol design and implementation

## Projects in This Phase

This phase consists of three main projects:

1. **TCP Multi-threaded Chat Room**: A real-time chat application with multiple concurrent users
2. **HTTP Static File Server**: A web server that serves static files with proper HTTP handling
3. **TCP File Transfer Server**: A server for uploading and downloading files over TCP

Each project builds network programming skills with increasing complexity.

## Network Programming Concepts Covered

- **Socket Programming**: Creating, binding, connecting, and using TCP/UDP sockets
- **Concurrent Server Models**: Thread-per-connection vs event-driven architectures
- **Network Protocols**: Understanding TCP/UDP, HTTP/1.1, and custom protocol design
- **I/O Multiplexing**: Using select/poll/epoll for efficient I/O handling (advanced)
- **Network Security**: Basic principles of secure network programming
- **Serialization**: Converting data to network-transmissible formats

## Development Environment

Continue using the Docker development environment which provides the necessary network tools:

```bash
# Enter development container
./scripts/docker-dev.sh exec

# Navigate to project root
cd /app

# Build Phase 3 projects
cd build
cmake ..
make -j4
```

## Testing Strategy

For network programming projects, testing involves:

1. Functional tests with actual network connections
2. Concurrency tests with multiple simultaneous clients
3. Load and stress testing to evaluate performance
4. Error condition testing (connection drops, timeouts)
5. Security and edge case testing

Run comprehensive tests with:
```bash
./scripts/docker-dev.sh test
```

## Next Steps

Begin with the [TCP Multi-threaded Chat Room](./tcp-chat-room.md) project to explore real-time network communication!