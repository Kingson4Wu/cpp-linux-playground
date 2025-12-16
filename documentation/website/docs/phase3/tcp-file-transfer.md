---
sidebar_position: 4
---

# TCP File Transfer Server Implementation

Build a TCP-based file transfer server that allows clients to upload and download files. This project explores binary data transmission, file I/O operations, and protocol design for non-text data.

## Project Structure

The file transfer server is organized as follows:

```
phase3/tcp-file-transfer/
├── CMakeLists.txt
├── include/
│   └── file_server.h  # File server interface and classes
└── src/
    └── file_server.cpp  # Implementation of file transfer functionality
```

## Learning Objectives

During this project, you will:

- Implement binary data transmission over TCP
- Design custom protocols for file operations
- Handle large file transfers with proper buffering
- Manage file I/O operations safely across network connections
- Implement file transfer progress tracking
- Apply security measures for file system access
- Design efficient packet-based communication

## Core Concepts

### Protocol Design

Design a structured protocol for file operations:

- **Command Packets**: Define message types for upload, download, list files, etc.
- **Header Format**: Include length, command type, and optional flags
- **Data Format**: Define how file data is segmented and transmitted
- **Error Handling**: Protocol-level error responses

### Binary Data Transmission

Unlike text protocols, binary protocols must:

- Specify exact message lengths in headers
- Use binary-safe encoding for metadata
- Handle partial messages and reassembly
- Implement proper endianness handling for multi-byte values

## Implementation Architecture

### FileServer Interface

```cpp
class FileServer {
public:
    explicit FileServer(int port, const std::string& storage_path);
    void start();
    void stop();
    
private:
    int server_fd_;
    int port_;
    std::string storage_path_;
    std::atomic<bool> running_;
    std::vector<std::thread> client_threads_;
    
    void accept_connections();
    void handle_client(int client_fd);
    void process_command(int client_fd);
    
    // File operation handlers
    bool handle_upload(int client_fd);
    bool handle_download(int client_fd);
    bool handle_list_files(int client_fd);
};
```

### Protocol Message Format

```cpp
enum class CommandType : uint8_t {
    UPLOAD = 1,
    DOWNLOAD = 2,
    LIST_FILES = 3,
    DELETE_FILE = 4,
    RESPONSE = 5
};

struct FileCommand {
    CommandType type;
    uint32_t length;      // Length of data that follows
    std::string filename; // Optional, depending on command
    std::vector<char> data; // Command-specific data
};

struct FileResponse {
    uint8_t status;       // 0 = success, non-zero = error code
    uint32_t length;      // Length of optional message
    std::string message;  // Optional error message or file list
};
```

## Implementation Requirements

### Core Features

1. **File Upload**: Clients can upload files to the server
2. **File Download**: Clients can download files from the server
3. **File Listing**: Clients can request a list of available files
4. **Protocol Implementation**: Structured binary protocol with headers
5. **Security**: Prevent access to files outside the storage directory
6. **Concurrent Access**: Handle multiple simultaneous file transfers

### Transfer Protocol

Design a reliable transfer protocol:

- **Upload**: Client sends file in chunks with size indicators
- **Download**: Server sends file in chunks with size indicators
- **Checksum Verification**: Optional integrity checking for large files (advanced)
- **Resume Support**: Ability to resume interrupted transfers (advanced)

### Sample Implementation Approach

```cpp
class FileServer {
private:
    struct TransferSession {
        std::string filename;
        size_t file_size;
        size_t bytes_transferred;
        std::fstream file_stream;
    };
    
    std::string storage_path_;
    std::unordered_map<int, TransferSession> active_transfers_;
    std::mutex transfers_mutex_;
    
    bool read_command_header(int client_fd, FileCommand& cmd);
    bool send_response(int client_fd, const FileResponse& resp);
    bool transfer_file_data(int client_fd, std::fstream& file, size_t size);
    
public:
    void handle_client(int client_fd);
};
```

## Binary Protocol Implementation

### Message Framing

Implement proper message framing since TCP is a stream protocol:

```cpp
// Example: Read a complete message
bool read_message(int fd, std::vector<char>& buffer) {
    uint32_t length;
    if (::recv(fd, &length, sizeof(length), 0) != sizeof(length)) {
        return false;
    }
    length = ntohl(length);  // Convert from network byte order
    
    buffer.resize(length);
    size_t received = 0;
    while (received < length) {
        ssize_t result = ::recv(fd, buffer.data() + received, 
                               length - received, 0);
        if (result <= 0) return false;
        received += result;
    }
    return true;
}
```

## Security Considerations

### File System Access

- Validate filenames to prevent directory traversal attacks
- Use absolute paths when checking access permissions
- Limit file operations to within the designated storage directory
- Implement proper access controls (optional advanced feature)

### Resource Management

- Limit the number of concurrent transfers
- Implement timeouts for inactive connections
- Validate file sizes to prevent resource exhaustion
- Close files properly even in error conditions

## Performance Considerations

- Use appropriate buffer sizes for file I/O (typically 64KB-1MB)
- Consider memory mapping for very large files
- Optimize network buffer usage to minimize system calls
- Implement proper flow control to avoid overwhelming the receiver
- Consider compression for text-based or compressible files (advanced)

## Testing Strategy

Test your file transfer server with:

- File uploads of various sizes
- File downloads with verification (compare checksums)
- Concurrent uploads and downloads
- Error conditions (network interruption, invalid paths)
- Large file transfers
- Different file types (text, binary, executables)

## Usage Example

```cpp
#include "file_server.h"
#include <iostream>

int main() {
    try {
        // Store files in the ./storage directory, listen on port 9090
        FileServer server(9090, "./storage");
        std::cout << "File server starting on port 9090..." << std::endl;
        
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

## Client Implementation Considerations

Your server should work with a custom client that implements the same protocol, but for testing you might also implement:

- Command-line client for uploads/downloads
- Protocol testing tools
- Performance benchmarking clients

## Advanced Features (Optional)

Consider implementing additional capabilities:

1. **File Permissions**: Implement user authentication and file permissions
2. **Transfer Encryption**: Use TLS/SSL for secure transfers
3. **Resume Interrupted Transfers**: Allow clients to resume from partial transfers
4. **File Compression**: Compress files during transfer to reduce bandwidth
5. **Transfer Speed Control**: Implement bandwidth limiting
6. **File Deduplication**: Prevent duplicate file storage

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# Create storage directory
mkdir -p storage

# Start the file transfer server
./phase3/tcp-file-transfer/file_server

# Test with custom client implementation or scripts
```

## Next Steps

After completing the file transfer server, move to [Phase 4: Comprehensive Practice](../phase4/intro.md) to build complete applications that combine all your skills.