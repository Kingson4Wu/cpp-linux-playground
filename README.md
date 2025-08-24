# C++ Linux Backend Development Playground

[![Build Status](https://github.com/your-username/cpp-linux-playground/workflows/CI/badge.svg)](https://github.com/your-username/cpp-linux-playground/actions)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

A comprehensive hands-on C++ learning playground for Linux, covering modern C++ syntax, system programming, concurrent programming, network programming, and performance optimization.

## 🎯 Project Goals

This repository provides a structured learning path for backend engineers to master C++ through progressive hands-on projects. By completing these projects, you will:

- Master modern C++ (C++11/14/17/20) core features
- Become proficient in Linux system calls, file I/O, and process/thread management
- Develop high-performance network services (TCP/HTTP, multiplexing)
- Learn performance optimization and engineering practices (memory pools, lock optimization, logging libraries, build tools)
- Build a portfolio of backend C++ projects to showcase your skills

## 🚀 Project Roadmap

### **Phase 1: Syntax & Tools Familiarization (Beginner)**

Foundational projects to get comfortable with C++ syntax, build systems, and debugging tools.

#### Command-Line Tools Suite
- Implementation of simplified versions of `ls`, `grep`, and `wc`
- Key learning points: File I/O, string processing, regular expressions, CMake
- Extensions: Multi-threaded processing for large files

#### JSON Parser
- Custom JSON parser implementation
- Key learning points: STL containers, recursive descent parsing, exception handling
- Extensions: Serialization and deserialization support

#### Multi-threaded Logger
- Thread-safe logging library (mini-spdlog)
- Key learning points: Multi-threading synchronization, file buffering, timestamps
- Extensions: Asynchronous logging, log rotation

### **Phase 2: System Programming & Multi-threading (Intermediate)**

Projects focused on Linux system calls, process/thread management, and memory management.

#### Multi-threaded Downloader
- Concurrent file downloader (HTTP GET)
- Key learning points: pthread/C++11 threads, condition variables, sockets
- Extensions: Resume downloads, rate limiting

#### Process Manager
- Simplified implementation of `ps` or `top`
- Key learning points: Reading `/proc`, system calls, signal handling
- Extensions: Process tree visualization

#### Memory Pool Allocator
- Custom memory allocator (alternative to new/delete)
- Key learning points: Memory management, alignment, free lists
- Extensions: Performance benchmarking against standard allocators

### **Phase 3: Network Programming (Core)**

Projects to develop stable, high-performance C++ network services.

#### Multi-threaded Chat Room (TCP)
- Multi-client chat application with message broadcasting
- Key learning points: Sockets, epoll, multi-threading synchronization
- Extensions: Protocol buffers, command parsing

#### HTTP Static File Server
- HTTP server supporting GET/POST requests for static files
- Key learning points: HTTP protocol parsing, state machines, non-blocking I/O
- Extensions: Thread pools, keep-alive connections, gzip compression

#### TCP File Transfer Server
- Server supporting file upload and download
- Key learning points: TCP socket programming, multi-threading, file I/O, custom protocols
- Extensions: Large file chunked transfer, resume downloads

### **Phase 4: Comprehensive Projects (Advanced)**

Integrated projects combining C++ language features, system programming, network programming, and performance optimization.

#### Mini-Redis (KV Storage Engine)
- In-memory key-value storage with Redis-like functionality
- Key learning points: Hash table implementation, persistence (AOF/snapshot), multiplexing (epoll)
- Extensions: LRU cache eviction, transactions, multi-threading

#### Mini-Search (Search Engine)
- Text search engine with inverted index
- Key learning points: Inverted indexing, file I/O, multi-threaded index building
- Extensions: HTTP API for querying

#### High-concurrency Web Crawler
- Concurrent web crawler with thread pool and task queue
- Key learning points: HTTP client implementation, thread pools, concurrent programming
- Extensions: Rate limiting, proxy support, resume crawling

## 🛠 Technology Stack

- **Language**: Modern C++ (C++11/14/17/20)
- **Platform**: Linux (Ubuntu/Debian recommended)
- **Build Tools**: CMake, Makefile
- **Debugging & Analysis**: GDB, Valgrind, Perf, AddressSanitizer
- **Networking**: Socket API, epoll, Reactor pattern
- **Testing**: Google Test framework
- **Containerization**: Docker for consistent development environment

## 🏗 Development Environment Setup

### Prerequisites

- Docker (recommended for consistent environment)
- Git
- Basic understanding of C++ and Linux

### Quick Start with Docker

```bash
# Clone the repository
git clone https://github.com/your-username/cpp-linux-playground.git
cd cpp-linux-playground

# Build Docker image
./scripts/docker-dev.sh build

# Start container
./scripts/docker-dev.sh run

# Enter container
./scripts/docker-dev.sh exec

# Build and run specific tools
./scripts/docker-dev.sh run-ls
./scripts/docker-dev.sh run-grep
./scripts/docker-dev.sh run-wc

# Run all tests
./scripts/docker-dev.sh test

# Debug with GDB
./scripts/docker-dev.sh debug

# Generate code coverage report
./scripts/docker-dev.sh coverage

# Stop container
./scripts/docker-dev.sh stop

# Clean up
./scripts/docker-dev.sh clean
```

**Note**: To improve development efficiency, different tasks use separate build directories:
- `run-*` and `debug` commands use the `build/` directory
- `test` command uses the `build-test/` directory
- `coverage` command uses the `build_coverage/` directory

This prevents unnecessary recompilation when switching between tasks.

## 🧪 Testing & Quality Assurance

### Unit Testing

All projects include comprehensive unit tests using Google Test framework:

```bash
# Run all tests
./scripts/docker-dev.sh test

# Run specific test suite
./scripts/docker-dev.sh exec bash -c "cd /app/build && ./tests/phase1/cli-tools/ls_test"
```

### Code Coverage

To ensure code quality and test completeness, the project supports generating code coverage reports:

```bash
# Generate coverage report
./scripts/docker-dev.sh coverage
```

Coverage reports are generated in `build_coverage/coverage/`:
- HTML format: `build_coverage/coverage/index.html`
- XML format: `build_coverage/coverage/coverage.xml`

Target coverage: 90%+ to ensure core functionality is thoroughly tested.

### Static Analysis & Sanitizers

The project supports various static analysis tools and sanitizers:

```bash
# Build with AddressSanitizer
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
cmake --build build-asan

# Run with AddressSanitizer
./build-asan/phase1/cli-tools/my_ls
```

## 📊 Project Components Overview

### Phase 1: CLI Tools
- **my_ls**: Enhanced directory listing utility
- **my_grep**: Pattern matching tool with regex support
- **my_wc**: Word, line, and character counting utility
- **JSON Parser**: Custom JSON parsing library
- **Logger**: Thread-safe logging library

### Phase 2: System Programming
- **Threaded Downloader**: Multi-threaded HTTP file downloader
- **Process Manager**: System process monitoring utility
- **Memory Pool**: Custom memory allocator implementation

### Phase 3: Network Programming
- **TCP Chat Room**: Multi-user chat server and client
- **HTTP Server**: Static file web server with thread pool
- **TCP File Transfer**: File upload/download server and client

### Phase 4: Advanced Applications
- **Mini-Redis**: Redis-compatible in-memory key-value store
- **Mini-Search**: Text search engine with inverted index
- **Web Crawler**: High-concurrency web crawling framework

## 📚 Learning Resources

### Books
- "Effective C++" / "More Effective C++" - Scott Meyers
- "C++ Primer (5th Edition)"
- "Advanced Programming in the UNIX Environment" - W. Richard Stevens
- "UNIX Network Programming" - W. Richard Stevens
- "Linux High Performance Server Programming" - Shuang You

### Online Resources
- [CppReference](https://en.cppreference.com)
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features)
- [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines)

## 🤝 Contributing

This project is primarily for educational purposes, but contributions are welcome:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

Please ensure all tests pass before submitting a pull request.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Inspired by various C++ learning resources and real-world backend development practices
- Special thanks to open-source projects that influenced the design of these implementations