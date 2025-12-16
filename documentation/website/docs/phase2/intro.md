---
sidebar_position: 1
---

# Phase 2: System Programming

Phase 2 focuses on Linux system programming, where you'll learn to interact directly with the operating system through system calls. This phase covers process management, thread synchronization, memory management, and other low-level programming concepts.

## Learning Objectives

By the end of Phase 2, you will:

- Master Linux system calls for file I/O, process management, and inter-process communication
- Implement custom memory management solutions
- Understand threading and synchronization primitives
- Create high-performance, system-level applications
- Apply profiling and debugging techniques to system code
- Understand memory mapping and shared memory concepts

## Projects in This Phase

This phase consists of three main projects:

1. **Multi-threaded Downloader**: A concurrent file download application
2. **Process Management Tool**: A simplified version of `ps`/`top` for process monitoring
3. **Memory Pool Allocator**: A custom memory allocator with performance optimization

Each project builds on the C++ knowledge from Phase 1 while introducing system-level programming concepts.

## System Programming Concepts Covered

- **POSIX Threading**: Using pthreads or std::thread for concurrency
- **System Calls**: File operations (open, read, write), process control (fork, exec, wait), and synchronization primitives
- **Memory Management**: Custom allocators, memory mapping (mmap), and memory layout
- **Inter-Process Communication**: Shared memory, pipes, and signals
- **Performance Optimization**: Minimizing system call overhead and optimizing I/O operations

## Development Environment

Continue using the Docker development environment to ensure consistent system call availability:

```bash
# Enter development container
./scripts/docker-dev.sh exec

# Navigate to project root
cd /app

# Build Phase 2 projects
cd build
cmake ..
make -j4
```

## Testing Strategy

For system programming projects, testing involves:

1. Functional tests that verify correct behavior
2. Stress tests to check resource management under load
3. Concurrency tests for thread safety
4. Memory leak detection using valgrind or AddressSanitizer

Run comprehensive tests with:
```bash
./scripts/docker-dev.sh test
```

## Next Steps

Begin with the [Multi-threaded Downloader](./threaded-downloader.md) project to explore concurrent file operations!