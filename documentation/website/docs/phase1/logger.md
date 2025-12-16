---
sidebar_position: 4
---

# Multi-threaded Logger Implementation

Develop a high-performance, thread-safe logging system that can handle concurrent write requests from multiple threads. This project introduces you to concurrent programming concepts in C++.

## Project Structure

The logger is organized as follows:

```
phase1/logger/
├── CMakeLists.txt
├── include/
│   └── logger.h    # Logger interface and classes
└── src/
    └── logger.cpp  # Implementation of logging functionality
```

## Learning Objectives

During this project, you will:

- Implement thread-safe data structures using mutexes and condition variables
- Understand producer-consumer patterns
- Explore lock-free programming concepts (optional advanced feature)
- Design asynchronous systems
- Apply RAII (Resource Acquisition Is Initialization) principles
- Practice proper C++ concurrency patterns

## Core Concepts

### Thread Safety Requirements

Your logger must safely handle concurrent access from multiple threads:

- Multiple threads can log messages simultaneously
- Log messages must not be corrupted or interleaved
- Performance should not degrade significantly with many threads
- Critical sections should be minimized

### Asynchronous Logging Pattern

To minimize logging overhead on application threads, implement an asynchronous design:

1. Application threads write log entries to a queue
2. A dedicated logger thread processes the queue
3. The logger thread writes entries to the output destination

## Implementation Architecture

### Logging Interface

Provide a simple, easy-to-use interface:

```cpp
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3
};

class Logger {
public:
    static void log(LogLevel level, const std::string& message);
    static void set_output_file(const std::string& filepath);
    static void set_level(LogLevel level);
    
    // Convenience macros/functions
    static void debug(const std::string& msg);
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);
};
```

### Queue-Based Architecture

Implement a thread-safe queue for message buffering:

```cpp
template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex mtx_;
    std::queue<T> queue_;
    std::condition_variable cv_;

public:
    void push(T item);
    T pop();  // Blocks until item is available
    bool try_pop(T& item);  // Non-blocking
    bool empty() const;
};
```

### Log Entry Structure

Define a structure to hold log information:

```cpp
struct LogEntry {
    LogLevel level;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    std::thread::id thread_id;
};
```

## Implementation Requirements

### Core Features

1. **Thread-Safe Operations**: All logging operations must be safe to call from multiple threads
2. **Log Levels**: Support different severity levels with filtering capability
3. **Formatted Output**: Include timestamps, thread IDs, and log levels in output
4. **File Output**: Support writing to specified output files
5. **Performance**: Minimal overhead on calling threads

### Sample Implementation Approach

```cpp
class Logger {
private:
    static std::mutex config_mtx_;
    static std::string output_file_;
    static LogLevel min_level_;
    static ThreadSafeQueue<LogEntry> log_queue_;
    static std::thread worker_thread_;
    static std::atomic<bool> running_;
    
    static void worker_loop();  // Dedicated logging thread function
    
public:
    static void log(LogLevel level, const std::string& message);
    static void initialize(const std::string& filepath, LogLevel level);
    static void shutdown();
};
```

### Thread Safety Techniques

Use these synchronization primitives:

- **Mutexes**: Protect shared data structures
- **Condition Variables**: Coordinate between producer and consumer threads
- **Atomic Operations**: For simple shared state like shutdown flags
- **RAII Locks**: Use `std::lock_guard` and `std::unique_lock` for automatic lock management

## Advanced Features (Optional)

Consider implementing these additional capabilities:

1. **Log Rotation**: Automatically rotate log files when they reach a certain size
2. **Asynchronous Flushing**: Ensure all pending messages are written before shutdown
3. **Pattern Formatting**: Allow customizable log message formats
4. **Multiple Outputs**: Support logging to both files and console

## Performance Considerations

- Minimize lock contention by keeping critical sections small
- Use move semantics to avoid unnecessary copies
- Consider using lock-free queues for high-throughput scenarios
- Profile to verify that logging has minimal impact on application performance

## Testing Strategy

Write tests that verify:

- Thread safety with multiple concurrent loggers
- Correct message ordering (or acceptable ordering constraints)
- Proper shutdown handling
- Performance under load
- Error handling for file I/O issues

## Usage Example

```cpp
#include "logger.h"

int main() {
    // Initialize logger
    Logger::initialize("app.log", LogLevel::INFO);
    
    // Multiple threads logging concurrently
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([i]() {
            Logger::info("Message from thread " + std::to_string(i));
            Logger::debug("Debug message from thread " + std::to_string(i));  // Won't appear due to level
        });
    }
    
    // Wait for threads and shutdown
    for (auto& t : threads) {
        t.join();
    }
    
    Logger::shutdown();
    return 0;
}
```

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# The logger should be integrated into the main build
# Test with multi-threaded programs to verify thread safety
```

## Next Steps

After completing the logger, review the [Development Standards](../standards/code-style.md) to understand the coding practices used throughout the project.