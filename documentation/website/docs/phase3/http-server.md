---
sidebar_position: 3
---

# HTTP Static File Server Implementation

Build a high-performance HTTP server that serves static files to clients. This project explores HTTP protocol handling, file I/O operations, and proper web server behavior.

## Project Structure

The HTTP server is organized as follows:

```
phase3/http-server/
├── CMakeLists.txt
├── include/
│   └── http_server.h  # HTTP server interface and classes
└── src/
    └── http_server.cpp  # Implementation of HTTP server functionality
```

## Learning Objectives

During this project, you will:

- Parse and respond to HTTP/1.1 requests
- Handle various HTTP methods (GET, HEAD primarily)
- Serve different file types with proper MIME types
- Implement HTTP headers and status codes correctly
- Handle file paths securely and prevent directory traversal attacks
- Apply efficient file serving strategies including memory mapping
- Understand web server architecture and request handling

## Core Concepts

### HTTP Protocol Fundamentals

The HTTP protocol consists of:

- **Request**: Method, path, HTTP version, headers, and optional body
- **Response**: Status code, headers, and body
- **Headers**: Key-value pairs for metadata (Content-Type, Content-Length, etc.)

Example request:
```
GET /index.html HTTP/1.1
Host: localhost:8080
User-Agent: Custom-Client/1.0
```

Example response:
```
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 1234

[HTML content here]
```

### File Serving Architecture

The server should:

1. Accept TCP connections from clients
2. Parse HTTP requests from the connection
3. Validate request paths (security) 
4. Generate appropriate HTTP responses
5. Serve file content or error pages
6. Close connections properly

## Implementation Architecture

### HTTPServer Interface

```cpp
class HTTPServer {
public:
    explicit HTTPServer(int port, const std::string& doc_root);
    void start();
    void stop();
    
private:
    int server_fd_;
    int port_;
    std::string doc_root_;
    std::atomic<bool> running_;
    std::vector<std::thread> worker_threads_;
    
    void accept_connections();
    void handle_client(int client_fd);
    HTTPResponse process_request(const HTTPRequest& request);
    
    // Utility methods
    std::string get_mime_type(const std::string& path);
    bool is_valid_path(const std::string& path);
    std::string get_file_path(const std::string& requested_path);
};
```

### HTTP Request/Response Structures

```cpp
struct HTTPRequest {
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

struct HTTPResponse {
    std::string version;
    int status_code;
    std::string status_text;
    std::map<std::string, std::string> headers;
    std::string body;
};
```

## Implementation Requirements

### Core Features

1. **HTTP Request Parsing**: Parse method, path, headers from incoming requests
2. **Static File Serving**: Read files from the document root and serve them
3. **MIME Type Detection**: Set appropriate Content-Type headers for different file types
4. **HTTP Status Codes**: Properly handle success, error, and redirect responses
5. **Security**: Prevent directory traversal and validate file paths
6. **Concurrent Access**: Handle multiple concurrent client requests

### Supported HTTP Status Codes

- `200 OK`: File found and served successfully
- `400 Bad Request`: Invalid HTTP request
- `404 Not Found`: File not found in the document root
- `403 Forbidden`: Requested path is outside the document root
- `500 Internal Server Error`: Server-side error

### File Types and MIME Types

Map file extensions to proper MIME types:

- `.html`, `.htm` → `text/html`
- `.css` → `text/css`
- `.js` → `application/javascript`
- `.json` → `application/json`
- `.png` → `image/png`
- `.jpg`, `.jpeg` → `image/jpeg`
- `.txt` → `text/plain`

### Sample Implementation Approach

```cpp
class HTTPServer {
private:
    struct RequestHandler {
        static HTTPResponse handle_get(const HTTPRequest& req, const std::string& doc_root);
        static HTTPResponse handle_head(const HTTPRequest& req, const std::string& doc_root);
        static HTTPResponse create_error_response(int code, const std::string& message);
    };
    
    std::string doc_root_;
    
    HTTPResponse serve_file(const std::string& path);
    void send_response(int client_fd, const HTTPResponse& response);
    
public:
    void handle_client(int client_fd);
};
```

## Security Considerations

### Path Validation

Prevent directory traversal attacks by ensuring requested paths don't escape the document root:

- Sanitize URLs by resolving `.` and `..` components
- Use absolute paths to verify the requested file is within the document root
- Reject requests that could access files outside the allowed directory

### Input Validation

- Validate HTTP method and path for expected patterns
- Check header sizes to prevent memory exhaustion
- Limit request body sizes (though not typically needed for GET requests)

## Performance Considerations

- Use memory mapping for large files to avoid copying
- Implement appropriate buffering for file I/O
- Consider caching for frequently accessed files
- Use efficient string operations for header parsing
- Optimize for the common case of successful file serving

## Testing Strategy

Test your HTTP server with:

- Valid file requests (various file types)
- Invalid paths (404 errors)
- Directory traversal attempts (403 errors)
- HTTP header validation
- Concurrent requests
- Large file serving
- Different HTTP clients (browser, curl, etc.)

## Usage Example

```cpp
#include "http_server.h"
#include <iostream>

int main() {
    try {
        // Serve files from current directory on port 8080
        HTTPServer server(8080, "./www");
        std::cout << "HTTP server starting on port 8080, serving from ./www..." << std::endl;
        
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

Test your server with various clients:

```bash
# With curl
curl http://localhost:8080/index.html

# With browser
open http://localhost:8080/index.html

# With wget
wget -O - http://localhost:8080/style.css
```

## Advanced Features (Optional)

Consider implementing additional capabilities:

1. **Index Files**: Serve index.html when directory is requested
2. **Range Requests**: Support HTTP range requests for partial content
3. **Gzip Compression**: Compress responses for text-based content
4. **Caching Headers**: Add appropriate cache control headers
5. **Virtual Hosts**: Support multiple document roots based on Host header

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# Create a document root with some files
mkdir -p www
echo "<html><body>Hello World</body></html>" > www/index.html
echo "body { background-color: white; }" > www/style.css

# Start the HTTP server
./phase3/http-server/http_server

# Test with browser or curl at http://localhost:8080/
```

## Next Steps

After completing the HTTP server, implement the [TCP File Transfer Server](./tcp-file-transfer.md) to explore binary data transmission.