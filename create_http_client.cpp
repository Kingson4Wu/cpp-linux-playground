#include <fstream>
#include <iostream>

int main() {
    std::ofstream out("/Users/kingsonwu/programming/cpp_src/cpp-linux-playground/phase4/crawler/src/http_client.cpp");
    
    // Write the correct content
    out << R"(#include "http_client.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cerrno>
#include <sys/select.h>
#include <algorithm>

namespace crawler {

namespace {

struct HttpClientImpl {
    static std::unique_ptr<HttpClient::HttpResponse> ParseResponse(const std::string& response_data) {
        auto response = std::make_unique<HttpClient::HttpResponse>();
        
        // Find the end of headers
        std::string::size_type header_end = response_data.find("\r\n\r\n");
        if (header_end == std::string::npos) {
            std::cerr << "Invalid HTTP response format" << std::endl;
            return nullptr;
        }
        
        // Parse status line
        std::string headers_str = response_data.substr(0, header_end);
        std::istringstream header_stream(headers_str);
        
        std::string status_line;
        std::getline(header_stream, status_line);
        
        // Parse status code (format: HTTP/1.1 200 OK)
        std::istringstream status_stream(status_line);
        std::string http_version;
        status_stream >> http_version >> response->status_code;
        
        // Parse headers
        std::string header_line;
        while (std::getline(header_stream, header_line)) {
            // Remove \r at the end if present
            if (!header_line.empty() && header_line.back() == '\r') {
                header_line.pop_back();
            }
            
            // Skip empty lines
            if (header_line.empty()) {
                continue;
            }
            
            // Split header into name and value
            std::string::size_type colon_pos = header_line.find(':');
            if (colon_pos != std::string::npos) {
                std::string name = header_line.substr(0, colon_pos);
                std::string value = header_line.substr(colon_pos + 1);
                
                // Trim whitespace
                name.erase(0, name.find_first_not_of(" \t"));
                name.erase(name.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                // Convert header name to lowercase
                std::transform(name.begin(), name.end(), name.begin(), 
                               [](unsigned char c) { return std::tolower(c); });
                
                response->headers[name] = value;
            }
        }
        
        // Parse body
        response->body = response_data.substr(header_end + 4);
        
        return response;
    }
};

} // anonymous namespace

HttpClient::HttpClient(int timeout_seconds) : timeout_seconds_(timeout_seconds) {}

HttpClient::~HttpClient() {}

std::unique_ptr<HttpClient::HttpResponse> HttpClient::Get(const std::string& url) {
    // Parse the URL
    UrlParser parser(url);
    if (!parser.IsValid()) {
        std::cerr << "Invalid URL: " << url << std::endl;
        return nullptr;
    }
    
    // Only support HTTP for now
    if (parser.GetProtocol() != "http") {
        std::cerr << "Only HTTP protocol is supported" << std::endl;
        return nullptr;
    }
    
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return nullptr;
    }
    
    // Resolve hostname
    struct hostent* host = gethostbyname(parser.GetHost().c_str());
    if (!host) {
        std::cerr << "Failed to resolve hostname: " << parser.GetHost() << std::endl;
        close(sock);
        return nullptr;
    }
    
    // Connect to server
    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(parser.GetPort());
    memcpy(&server_addr.sin_addr, host->h_addr_list[0], host->h_length);
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server: " << strerror(errno) << std::endl;
        close(sock);
        return nullptr;
    }
    
    // Prepare HTTP request
    std::ostringstream request_stream;
    request_stream << "GET " << parser.GetPath();
    if (!parser.GetQuery().empty()) {
        request_stream << "?" << parser.GetQuery();
    }
    request_stream << " HTTP/1.1\r\n";
    request_stream << "Host: " << parser.GetHost() << "\r\n";
    request_stream << "Connection: close\r\n";
    request_stream << "\r\n";
    
    std::string request = request_stream.str();
    
    // Send request
    ssize_t bytes_sent = send(sock, request.c_str(), request.length(), 0);
    if (bytes_sent < 0) {
        std::cerr << "Failed to send request: " << strerror(errno) << std::endl;
        close(sock);
        return nullptr;
    }
    
    // Receive response with timeout
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sock, &read_fds);
    
    struct timeval timeout;
    timeout.tv_sec = timeout_seconds_;
    timeout.tv_usec = 0;
    
    int select_result = select(sock + 1, &read_fds, nullptr, nullptr, &timeout);
    if (select_result <= 0) {
        if (select_result == 0) {
            std::cerr << "Timeout while receiving response" << std::endl;
        } else {
            std::cerr << "Error while waiting for response: " << strerror(errno) << std::endl;
        }
        close(sock);
        return nullptr;
    }
    
    // Receive response data
    std::string response_data;
    char buffer[4096];
    ssize_t bytes_received;
    
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        response_data += buffer;
    }
    
    if (bytes_received < 0) {
        std::cerr << "Error receiving response: " << strerror(errno) << std::endl;
        close(sock);
        return nullptr;
    }
    
    close(sock);
    
    // Parse response
    return HttpClientImpl::ParseResponse(response_data);
}

} // namespace crawler
)";

    return 0;
}
