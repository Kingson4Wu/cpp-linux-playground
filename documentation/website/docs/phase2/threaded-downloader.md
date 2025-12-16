---
sidebar_position: 2
---

# Multi-threaded Downloader Implementation

Build a high-performance, multi-threaded file download application that can download files in chunks simultaneously and reassemble them. This project introduces you to concurrent I/O operations and file management in a system programming context.

## Project Structure

The downloader is organized as follows:

```
phase2/threaded-downloader/
├── CMakeLists.txt
├── include/
│   └── downloader.h  # Downloader interface and classes
└── src/
    └── downloader.cpp  # Implementation of download functionality
```

## Learning Objectives

During this project, you will:

- Implement concurrent file downloading using threads
- Manage file I/O operations safely across multiple threads
- Handle HTTP/HTTPS requests using system libraries
- Implement proper synchronization for shared resources
- Design a progress tracking system for long-running operations
- Apply performance optimization techniques for I/O bound operations

## Core Concepts

### Concurrent Download Strategy

The downloader should split files into chunks and download them in parallel:

1. Determine file size using HTTP HEAD request
2. Divide file into N chunks based on thread count
3. Create download threads for each chunk
4. Write each chunk to the correct position in the output file
5. Reassemble chunks when all downloads complete

### Thread Safety Requirements

Ensure safe concurrent access to:

- The output file descriptor
- Shared state like download progress
- Network resources and connections
- Memory management for buffers

## Implementation Architecture

### Downloader Class Interface

```cpp
class Downloader {
public:
    struct DownloadConfig {
        std::string url;
        std::string output_path;
        int num_threads = 4;
        size_t chunk_size = 1024 * 1024;  // 1MB default
        std::chrono::seconds timeout = std::chrono::seconds(30);
    };
    
    struct DownloadProgress {
        size_t bytes_downloaded;
        size_t total_bytes;
        double percentage;
        std::chrono::steady_clock::time_point start_time;
    };
    
    static bool download(const DownloadConfig& config);
    static void set_progress_callback(std::function<void(const DownloadProgress&)> callback);
};
```

### Required System Calls and Libraries

- **Network Operations**: Use libcurl or raw sockets for HTTP requests
- **File I/O**: Use system calls like `open`, `write`, `lseek` for direct file operations
- **Threading**: Use `std::thread` with proper synchronization
- **Synchronization**: Use `std::mutex`, `std::condition_variable` for thread coordination

### Sample Implementation Approach

```cpp
class Downloader {
private:
    struct Chunk {
        size_t start_byte;
        size_t end_byte;
        std::string output_path;
    };
    
    static std::mutex file_mtx_;  // Protect file operations
    static std::atomic<size_t> total_downloaded_;
    
    static void download_chunk(const Chunk& chunk, const std::string& url);
    static size_t get_file_size(const std::string& url);
    
public:
    static bool download(const DownloadConfig& config);
};
```

## Implementation Requirements

### Core Features

1. **Chunked Downloads**: Split files into chunks and download concurrently
2. **Progress Reporting**: Track and report download progress
3. **Resume Capability**: Allow resuming interrupted downloads (optional advanced)
4. **Error Handling**: Properly handle network errors and retry logic
5. **Resource Management**: Proper cleanup of file handles and network connections

### Memory Management

- Use appropriately sized buffers for network I/O
- Consider memory mapping for large files to avoid copying
- Implement proper cleanup to prevent resource leaks

### HTTP Range Requests

Leverage HTTP range requests to download specific byte ranges:

```
Range: bytes=0-1023          # First 1024 bytes
Range: bytes=1024-2047       # Next 1024 bytes
```

## Performance Considerations

- Optimize chunk size based on network conditions
- Balance thread count with system resources
- Use memory mapping for efficient file I/O on large files
- Minimize system call overhead
- Consider using non-blocking I/O for better throughput

## Testing Strategy

Test your downloader with:

- Small and large files
- Various network conditions (slow connections, timeouts)
- Multiple concurrent downloads
- Files that change during download
- Network interruptions and recovery

## Usage Example

```cpp
#include "downloader.h"

int main() {
    Downloader::DownloadConfig config;
    config.url = "https://example.com/largefile.zip";
    config.output_path = "./largefile.zip";
    config.num_threads = 8;
    
    // Set up progress callback
    Downloader::set_progress_callback([](const auto& progress) {
        std::cout << "Downloaded: " << progress.percentage << "%" << std::endl;
    });
    
    bool success = Downloader::download(config);
    if (success) {
        std::cout << "Download completed successfully!" << std::endl;
    } else {
        std::cout << "Download failed!" << std::endl;
    }
    
    return 0;
}
```

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# The downloader should be integrated into the main build
# Test with various file sizes and network conditions
```

## Next Steps

After completing the downloader, move on to the [Process Management Tool](./process-manager.md) to explore system process monitoring.