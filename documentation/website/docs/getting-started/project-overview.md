---
sidebar_position: 2
---

# Project Overview

The Linux C++ Backend Development Playground is structured into four progressive phases, each designed to build upon the previous one and introduce increasingly complex concepts and challenges.

## Project Structure

The project follows this organized structure:

```
cpp-linux-playground/
├── CMakeLists.txt      # Root CMake configuration managing all sub-projects
├── phase1/             # Phase 1: Syntax and Tool Familiarization
│   ├── cli-tools/      # Command-line tools (ls, grep, wc)
│   ├── json-parser/    # JSON parsing library
│   └── logger/         # Multi-threaded logging system
├── phase2/             # Phase 2: System Programming
│   ├── memory-pool/    # Custom memory allocator
│   ├── process-manager/ # Process management tools
│   └── threaded-downloader/ # Multi-threaded download system
├── phase3/             # Phase 3: Network Programming
│   ├── tcp-chat-room/   # Multi-threaded TCP chat application
│   ├── http-server/     # HTTP static file server
│   └── tcp-file-transfer/ # TCP file transfer protocol
├── phase4/             # Phase 4: Comprehensive Practice
│   ├── crawler/         # High-concurrency web crawler
│   ├── mini-redis/      # Redis-like key-value store
│   └── mini-search/     # Search engine implementation
├── docker/             # Docker configuration files
├── scripts/            # Automation scripts
└── docs/               # Additional documentation
```

## Phase Progression

### Phase 1: Syntax and Tool Familiarization
Focuses on mastering fundamental C++20 syntax and common command-line tools:
- Implement simplified versions of Linux command-line utilities (ls, grep, wc)
- Build a JSON parser library
- Develop a multi-threaded logger system

### Phase 2: System Programming
Delves into Linux system calls and low-level programming:
- Create a multi-threaded downloader application
- Build a process management tool (similar to ps/top)
- Implement a custom memory pool allocator

### Phase 3: Network Programming
Covers networking fundamentals and protocols:
- Implement a TCP multi-threaded chat room
- Build an HTTP static file server
- Create a TCP file transfer server

### Phase 4: Comprehensive Practice
Combines all learned skills in complex, real-world projects:
- Build a mini-Redis key-value storage system
- Implement a mini-search engine
- Develop a high-concurrency web crawler

## Development Standards

Throughout the project, you'll follow industry-standard practices:

- **Code Style**: Consistent formatting using clang-format
- **Testing**: Comprehensive unit testing with Google Test
- **Documentation**: Clear, descriptive comments and documentation
- **Git Workflow**: Structured commit messages and branching strategies
- **Build Systems**: CMake for cross-platform builds

## Next Steps

Explore the individual phases to get started with specific learning modules:
- Proceed to [Phase 1: Syntax and Tool Familiarization](../phase1/intro.md)
- Or check out the [Docker Development Setup](./docker-setup.md) guide for detailed container-based development instructions.