#include "http_connection_handler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <sys/socket.h> // For recv, send
#include <unistd.h>     // For close
#include <cstring>      // For memset, strerror
#include <cerrno>       // For errno

namespace http_server {

HttpConnectionHandler::HttpConnectionHandler(int client_socket, const std::string& web_root)
    : client_socket_(client_socket), web_root_(web_root) {}

HttpConnectionHandler::~HttpConnectionHandler() {
    if (client_socket_ >= 0) {
        close(client_socket_);
    }
}

void HttpConnectionHandler::Handle() {
    HttpRequest request;
    if (!ReadRequest(request)) {
        HttpResponse response(400); // Bad Request
        SendResponse(response);
        return;
    }

    HttpResponse response;
    ProcessRequest(request, response);

    if (!SendResponse(response)) {
        std::cerr << "Failed to send response" << std::endl;
    }
}

bool HttpConnectionHandler::ReadRequest(HttpRequest& request) {
    // This is a simplified implementation that reads the entire request into a buffer.
    // A more robust implementation would handle large requests and streaming.
    char buffer[4096];
    ssize_t bytes_received = recv(client_socket_, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        return false;
    }

    buffer[bytes_received] = '\0';
    std::string request_str(buffer);

    return request.Parse(request_str);
}

void HttpConnectionHandler::ProcessRequest(const HttpRequest& request, HttpResponse& response) {
    // Only support GET method for now
    if (request.GetMethod() != "GET") {
        response.SetStatusCode(400); // Bad Request
        response.SetHeader("Content-Type", "text/plain");
        response.SetBody("Unsupported method");
        return;
    }

    std::string file_path = ResolveUri(request.GetUri());
    if (file_path.empty()) {
        response.SetStatusCode(400); // Bad Request
        response.SetHeader("Content-Type", "text/plain");
        response.SetBody("Invalid URI");
        return;
    }

    if (!std::filesystem::exists(file_path)) {
        response.SetStatusCode(404); // Not Found
        response.SetHeader("Content-Type", "text/plain");
        response.SetBody("File not found");
        return;
    }

    if (std::filesystem::is_directory(file_path)) {
        // Check for index.html
        std::string index_path = file_path + "/index.html";
        if (std::filesystem::exists(index_path) && std::filesystem::is_regular_file(index_path)) {
            file_path = index_path;
        } else {
            response.SetStatusCode(404); // Not Found
            response.SetHeader("Content-Type", "text/plain");
            response.SetBody("Directory listing not supported");
            return;
        }
    }

    ServeStaticFile(file_path, response);
}

bool HttpConnectionHandler::SendResponse(const HttpResponse& response) {
    std::string response_str = response.ToString();
    ssize_t total_bytes_sent = 0;
    size_t total_bytes_to_send = response_str.length();

    while (total_bytes_sent < total_bytes_to_send) {
        ssize_t bytes_sent = send(client_socket_, response_str.c_str() + total_bytes_sent, 
                                  total_bytes_to_send - total_bytes_sent, 0);
        if (bytes_sent <= 0) {
            return false;
        }
        total_bytes_sent += bytes_sent;
    }

    return true;
}

void HttpConnectionHandler::ServeStaticFile(const std::string& file_path, HttpResponse& response) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        response.SetStatusCode(500); // Internal Server Error
        response.SetHeader("Content-Type", "text/plain");
        response.SetBody("Failed to open file");
        return;
    }

    // Get file size
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file content
    std::string content(file_size, '\0');
    file.read(&content[0], file_size);

    // Set response
    response.SetStatusCode(200); // OK

    // Determine content type based on file extension
    std::filesystem::path path_obj(file_path);
    std::string extension = path_obj.extension().string();
    response.SetHeader("Content-Type", GetMimeType(extension));

    // Set content length
    response.SetHeader("Content-Length", std::to_string(file_size));

    // Set body
    response.SetBody(content);
}

std::string HttpConnectionHandler::ResolveUri(const std::string& uri) const {
    // Basic URI resolution and security check
    // This prevents directory traversal attacks like GET /../../../etc/passwd
    if (uri.empty() || uri[0] != '/') {
        return ""; // Invalid URI
    }

    // Normalize the URI by removing leading '/'
    std::string normalized_uri = uri.substr(1);

    // Resolve the path relative to web_root_
    // Use append to correctly combine paths
    std::filesystem::path root_path(web_root_);
    std::filesystem::path resolved_path = root_path / normalized_uri;

    // Canonicalize the path to resolve any '..' or '.'
    resolved_path = std::filesystem::weakly_canonical(resolved_path);

    // Check if the resolved path is within the web_root_ directory
    std::filesystem::path canonical_root = std::filesystem::weakly_canonical(root_path);
    if (resolved_path.string().find(canonical_root.string()) != 0) {
        return ""; // Path traversal detected
    }

    return resolved_path.string();
}

std::string HttpConnectionHandler::GetMimeType(const std::string& extension) const {
    // A simple mapping of file extensions to MIME types
    if (extension == ".html" || extension == ".htm") {
        return "text/html";
    } else if (extension == ".css") {
        return "text/css";
    } else if (extension == ".js") {
        return "application/javascript";
    } else if (extension == ".json") {
        return "application/json";
    } else if (extension == ".png") {
        return "image/png";
    } else if (extension == ".jpg" || extension == ".jpeg") {
        return "image/jpeg";
    } else if (extension == ".gif") {
        return "image/gif";
    } else if (extension == ".ico") {
        return "image/x-icon";
    } else if (extension == ".txt") {
        return "text/plain";
    } else if (extension == ".pdf") {
        return "application/pdf";
    } else {
        return "application/octet-stream"; // Default binary type
    }
}

} // namespace http_server
