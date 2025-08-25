# C++ Linux Backend Development Playground

[![Build Status](https://github.com/your-username/cpp-linux-playground/workflows/CI/badge.svg)](https://github.com/your-username/cpp-linux-playground/actions)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/language-C++20-blue.svg)](https://en.cppreference.com/)
[![Coverage](https://img.shields.io/badge/coverage-90%25-green.svg)](https://github.com/your-username/cpp-linux-playground)

A comprehensive hands-on C++ learning playground for Linux, covering modern C++ syntax, system programming, concurrent programming, network programming, and performance optimization through progressive, real-world projects.

## 🎯 Project Goals

This repository provides a structured learning path for backend engineers to master C++ through progressive hands-on projects. By completing these projects, you will:

- Master modern C++ (C++11/14/17/20) core features including smart pointers, lambdas, and concurrency primitives
- Become proficient in Linux system calls, file I/O, and process/thread management with practical applications
- Develop high-performance network services (TCP/HTTP, multiplexing) using industry-standard patterns
- Learn performance optimization and engineering practices (memory pools, lock optimization, logging libraries, build tools)
- Build a comprehensive portfolio of backend C++ projects to showcase your skills to potential employers
- Gain experience with professional development practices including testing, code coverage, and continuous integration

## 🚀 Project Roadmap

### **Phase 1: Syntax & Tools Familiarization (Beginner)**

Foundational projects to get comfortable with C++ syntax, build systems, and debugging tools.

#### Command-Line Tools Suite (ls, grep, wc)
- **Implementation**: Simplified versions of standard Unix utilities
- **Key Technologies**: File I/O operations, string processing, regular expressions (`<regex>`), command-line argument parsing
- **Learning Points**: 
  - Modern C++ features like range-based for loops, auto type deduction
  - STL containers and algorithms
  - Error handling with exceptions
  - CMake build system configuration
- **Advanced Features**: Multi-threaded processing for large files, colored output, recursive directory traversal
- **Detailed Features**:
  - Recursive directory traversal with depth limiting
  - File type detection and colorized output
  - Permission display in both symbolic and octal formats
  - Human-readable file sizes with appropriate units
  - Sorting options (name, size, modification time)
  - Filter by file type, extension, or pattern
  - Regular expression support with multiple syntax options
  - Case-insensitive and whole-word matching modes
  - Line numbering and context lines (before/after)
  - File and directory recursion with exclusion patterns
  - Binary file handling with automatic detection
  - Match highlighting in terminal output
  - Simultaneous counting of lines, words, and characters
  - Multi-byte character support (UTF-8)
  - File globbing and recursive directory processing
  - Custom delimiter specification for word counting
  - Progress indication for long-running operations

#### JSON Parser
- **Implementation**: Custom JSON parser implementation from scratch
- **Key Technologies**: Recursive descent parsing, RAII principles, variant types
- **Learning Points**:
  - Object-oriented design patterns
  - Template metaprogramming for type safety
  - Memory management with smart pointers
  - Exception handling for malformed input
- **Advanced Features**: JSON serialization/deserialization, streaming parser for large files
- **Detailed Features**:
  - Complete JSON specification compliance
  - DOM-style API for document manipulation
  - SAX-style streaming parser for memory efficiency
  - Type-safe accessors with automatic conversion
  - Error reporting with detailed location information
  - Pretty-printing with customizable formatting

#### Multi-threaded Logger
- **Implementation**: Thread-safe logging library (mini-spdlog) with multiple sinks
- **Key Technologies**: Mutexes, condition variables, atomic operations, circular buffers
- **Learning Points**:
  - Thread synchronization primitives
  - Producer-consumer pattern with queues
  - File I/O buffering for performance
  - Time formatting and timezone handling
- **Advanced Features**: Asynchronous logging with dedicated threads, log rotation, filtering by severity levels
- **Detailed Features**:
  - Multiple log levels (TRACE, DEBUG, INFO, WARN, ERROR, FATAL)
  - Synchronous and asynchronous logging modes
  - Multiple output destinations (console, file, syslog)
  - Log rotation by size and time
  - Thread-safe operation with minimal overhead
  - Custom formatting patterns and metadata

### **Phase 2: System Programming & Multi-threading (Intermediate)**

Projects focused on Linux system calls, process/thread management, and memory management.

#### Multi-threaded Downloader
- **Implementation**: Concurrent file downloader with resume capability (HTTP/HTTPS)
- **Key Technologies**: libcurl integration, thread pools, futures/promises
- **Learning Points**:
  - Asynchronous I/O operations
  - HTTP protocol implementation
  - Progress tracking and cancellation
  - Connection pooling for efficiency
- **Advanced Features**: Bandwidth throttling, proxy support, multipart downloads
- **Detailed Features**:
  - Multi-threaded download engine with connection pooling
  - HTTP/HTTPS support with certificate validation
  - Resume interrupted downloads with byte-range requests
  - Progress tracking with speed calculation
  - Bandwidth limiting and scheduling
  - Proxy support with authentication

#### Process Manager
- **Implementation**: Simplified implementation of `ps` or `top` with real-time monitoring
- **Key Technologies**: `/proc` filesystem parsing, signal handling, system metrics collection
- **Learning Points**:
  - Linux kernel interfaces
  - Memory and CPU usage calculation
  - Process lifecycle management
  - Real-time data visualization
- **Advanced Features**: Process tree visualization, resource usage alerts, historical data tracking
- **Detailed Features**:
  - Real-time process monitoring with resource usage
  - Process tree visualization and navigation
  - Signal sending and process control
  - Historical data collection and trend analysis
  - Alerting for abnormal resource consumption
  - Export to various formats (JSON, CSV, XML)

#### Memory Pool Allocator
- **Implementation**: Custom memory allocator with multiple allocation strategies
- **Key Technologies**: Free lists, buddy allocation, slab allocation
- **Learning Points**:
  - Memory fragmentation prevention
  - Allocation/deallocation performance optimization
  - Alignment requirements for different data types
  - Memory leak detection and debugging
- **Advanced Features**: Thread-local allocation caches, garbage collection integration
- **Detailed Features**:
  - Fixed-block and variable-block allocation strategies
  - Thread-local allocation caches for reduced contention
  - Memory usage statistics and fragmentation analysis
  - Custom alignment support for performance-critical data
  - Integration with standard allocation interfaces
  - Debug features for leak detection and corruption checking

### **Phase 3: Network Programming (Core)**

Projects to develop stable, high-performance C++ network services.

#### Multi-threaded Chat Room (TCP)
- **Implementation**: Multi-client chat application with message broadcasting and private messaging
- **Key Technologies**: TCP sockets, epoll/kqueue, thread pools, message framing
- **Learning Points**:
  - Reactor pattern implementation
  - Connection lifecycle management
  - Broadcast and unicast messaging
  - Protocol design and parsing
- **Advanced Features**: Message persistence, user authentication, chat room management
- **Detailed Features**:
  - Multi-user chat with private messaging support
  - User authentication and session management
  - Message persistence with database backend
  - Emoticon and rich text support
  - File transfer between users
  - Administration commands and user permissions

#### HTTP Static File Server
- **Implementation**: HTTP server supporting GET/POST requests for static files with advanced features
- **Key Technologies**: HTTP/1.1 protocol, MIME type detection, keep-alive connections
- **Learning Points**:
  - HTTP request/response parsing
  - File serving with proper headers
  - Directory listing and index files
  - Error handling (404, 500, etc.)
- **Advanced Features**: Gzip compression, range requests, virtual hosting, rate limiting
- **Detailed Features**:
  - HTTP/1.1 compliant static file server
  - MIME type detection and content negotiation
  - Keep-alive connections with timeout management
  - Directory listing with customizable templates
  - Custom error pages and redirects
  - Basic authentication and access control
  - Request logging and analytics

#### TCP File Transfer Server
- **Implementation**: Server supporting secure file upload and download with integrity checking
- **Key Technologies**: Custom binary protocol, checksum validation, streaming
- **Learning Points**:
  - Large file transfer optimization
  - Connection reliability and error recovery
  - File metadata handling
  - Progress reporting and cancellation
- **Advanced Features**: Resume downloads, directory synchronization, encryption
- **Detailed Features**:
  - Secure file transfer with checksum validation
  - Directory synchronization with delta detection
  - Resume interrupted transfers automatically
  - Compression and encryption options
  - Progress reporting with ETA calculation
  - Batch operations and scripting support

### **Phase 4: Comprehensive Projects (Advanced)**

Integrated projects combining C++ language features, system programming, network programming, and performance optimization.

#### Mini-Redis (KV Storage Engine)
- **Implementation**: In-memory key-value storage with Redis-like functionality and persistence
- **Key Technologies**: Hash tables, AOF persistence, snapshotting, RESP protocol
- **Learning Points**:
  - Data structure implementation (strings, lists, sets, hashes)
  - Persistence strategies and crash recovery
  - Command processing pipeline
  - Memory optimization techniques
- **Advanced Features**: Pub/Sub messaging, transactions, Lua scripting, clustering
- **Detailed Features**:
  - Redis-compatible in-memory key-value store
  - Support for strings, lists, sets, hashes, and sorted sets
  - Persistence with AOF and RDB snapshots
  - Pub/Sub messaging with pattern matching
  - Transactions with WATCH/MULTI/EXEC
  - Lua scripting for custom operations
  - Cluster support with sharding

#### Mini-Search (Search Engine)
- **Implementation**: Full-text search engine with inverted index and TF-IDF ranking
- **Key Technologies**: Inverted indexing, stemming algorithms, ranking algorithms
- **Learning Points**:
  - Text processing and tokenization
  - Index compression techniques
  - Query parsing and execution
  - Relevance scoring algorithms
- **Advanced Features**: Phrase matching, boolean operators, faceted search, spell correction
- **Detailed Features**:
  - Full-text search engine with inverted index
  - TF-IDF and BM25 ranking algorithms
  - Stemming and stop-word filtering
  - Phrase matching and boolean operators
  - Faceted search with aggregation
  - Spell correction and synonym expansion
  - REST API for integration

#### High-concurrency Web Crawler
- **Implementation**: Distributed web crawler with politeness policies and content analysis
- **Key Technologies**: HTTP client implementation, robots.txt parsing, content extraction
- **Learning Points**:
  - Distributed task scheduling
  - Rate limiting and domain politeness
  - Content deduplication
  - Storage backends (filesystem, database)
- **Advanced Features**: JavaScript rendering support, structured data extraction, sentiment analysis
- **Detailed Features**:
  - High-concurrency web crawler with politeness policies
  - Robots.txt compliance and sitemap parsing
  - Content extraction with XPath and CSS selectors
  - Duplicate detection and content hashing
  - Structured data extraction (microdata, JSON-LD)
  - Storage backends (filesystem, database, cloud)
  - Analytics and reporting dashboard

## 🛠 Technology Stack

### Languages & Standards
- **Primary**: Modern C++ (C++20 with fallback to C++17/14/11)
- **Build System**: CMake 3.20+
- **Testing**: Google Test framework
- **Documentation**: Doxygen-style comments

### Platforms & Tools
- **OS**: Linux (Ubuntu 24.04 LTS recommended)
- **Compilers**: GCC 13+/Clang 15+
- **Development**: Docker containers for consistent environments
- **Debugging**: GDB, Valgrind, AddressSanitizer, ThreadSanitizer
- **Performance**: Perf, strace, ltrace
- **Version Control**: Git with conventional commits

### Libraries & Dependencies
- **Networking**: POSIX sockets, libcurl (optional)
- **Threading**: Standard library threads, atomic operations
- **Memory**: Custom allocators, smart pointers
- **File I/O**: Standard library, memory-mapped files
- **Parsing**: Custom parsers, regular expressions

## 🏗 Development Environment Setup

### Prerequisites

- Docker (recommended for consistent environment)
- Git
- Basic understanding of C++ and Linux
- 8GB+ RAM recommended for compilation

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

# Open coverage report in browser
./scripts/docker-dev.sh open-coverage

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

All projects include comprehensive unit tests using Google Test framework with strict coverage requirements:

```bash
# Run all tests
./scripts/docker-dev.sh test

# Run specific test suite
./scripts/docker-dev.sh exec bash -c "cd /app/build-test && ./tests/phase1/cli-tools/ls_test"

# Run tests with verbose output
./scripts/docker-dev.sh exec bash -c "cd /app/build-test && ./tests/phase4/crawler/crawler_tests --gtest_output=xml:results.xml"
```

### Integration Testing

End-to-end testing of complete systems with real-world scenarios:

```bash
# Test HTTP server with curl
./scripts/docker-dev.sh exec bash -c "cd /app/build && ./phase3/http-server/http_server 8080 /var/www &"
./scripts/docker-dev.sh exec bash -c "curl http://localhost:8080/index.html"

# Test chat room with multiple clients
./scripts/docker-dev.sh exec bash -c "cd /app/build && ./phase3/tcp-chat-room/chat_server 8080 4 &"
./scripts/docker-dev.sh exec bash -c "cd /app/build && ./phase3/tcp-chat-room/chat_client localhost 8080 user1 &"
```

### Code Coverage

To ensure code quality and test completeness, the project supports generating code coverage reports with a target of 90%+ coverage:

```bash
# Generate coverage report
./scripts/docker-dev.sh coverage

# Open coverage report in browser
./scripts/docker-dev.sh open-coverage
```

Coverage reports are generated in `build_coverage/coverage/`:
- HTML format: `build_coverage/coverage/index.html` (interactive browser view)
- XML format: `build_coverage/coverage/coverage.xml` (CI/CD integration)
- Text summary: `build_coverage/coverage/coverage.txt` (terminal view)

### Static Analysis & Sanitizers

The project supports various static analysis tools and sanitizers for detecting bugs and performance issues:

```bash
# Build with AddressSanitizer for memory error detection
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON
cmake --build build-asan

# Run with AddressSanitizer
./build-asan/phase1/cli-tools/my_ls

# Build with ThreadSanitizer for race condition detection
cmake -S . -B build-tsan -DCMAKE_BUILD_TYPE=Debug -DENABLE_TSAN=ON
cmake --build build-tsan

# Run with ThreadSanitizer
./build-tsan/phase1/cli-tools/my_ls
```

## 📚 Learning Resources

### Books
- "Effective C++" / "More Effective C++" - Scott Meyers
- "C++ Primer (5th Edition)" - Stanley Lippman, Josée Lajoie, Barbara Moo
- "The C++ Programming Language (4th Edition)" - Bjarne Stroustrup
- "Advanced Programming in the UNIX Environment" - W. Richard Stevens
- "UNIX Network Programming" - W. Richard Stevens
- "Linux High Performance Server Programming" - Shuang You
- "C++ Concurrency in Action" - Anthony Williams

### Online Resources
- [CppReference](https://en.cppreference.com) - Comprehensive C++ standard library documentation
- [Modern C++ Features](https://github.com/AnthonyCalandra/modern-cpp-features) - C++11/14/17/20 feature reference
- [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines) - Official guidelines from Bjarne Stroustrup and Herb Sutter
- [C++ Tour](https://www.learncpp.com/) - A comprehensive tutorial covering the basics and advanced features of C++ for experienced programmers
- [Boost Documentation](https://www.boost.org/doc/) - Advanced C++ libraries (for reference)
- [Linux Man Pages](https://man7.org/linux/man-pages/) - System call and library documentation
- [HTTP Specification](https://tools.ietf.org/html/rfc7230) - RFC 7230-7237 for HTTP/1.1

### Video Courses
- "C++ Weekly" by Jason Turner (YouTube series)
- "Modern C++" by Klaus Iglberger (CppCon presentations)
- "Advanced Linux Programming" (various online platforms)
- "Network Programming" courses on Coursera/edX

## 🤝 Contributing

This project is primarily for educational purposes, but contributions are welcome to improve quality and add new features:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Ensure all tests pass (`./scripts/docker-dev.sh test`)
4. Add new tests for your feature
5. Update documentation as needed
6. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
7. Push to the branch (`git push origin feature/AmazingFeature`)
8. Open a Pull Request

### Code Quality Standards

- Follow existing code style and conventions
- Maintain 90%+ test coverage for new code
- Include documentation for public APIs
- Write clear, descriptive commit messages
- Ensure all CI checks pass before submitting

### Reporting Issues

- Use the GitHub issue tracker for bugs and feature requests
- Include detailed reproduction steps for bugs
- Specify your environment (OS, compiler version, etc.)
- Attach relevant logs or error messages

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

The MIT License is a permissive open-source license that allows for commercial use, modification, distribution, and patent use, with only a requirement for attribution.

## 🙏 Acknowledgments

- Inspired by various C++ learning resources and real-world backend development practices
- Special thanks to open-source projects that influenced the design of these implementations
- Community contributions and feedback that helped improve the quality of this educational resource
- The C++ community for continuously evolving the language and ecosystem