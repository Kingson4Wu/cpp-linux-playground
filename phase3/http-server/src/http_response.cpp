#include "http_response.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <set>
#include <vector>

namespace http_server {

HttpResponse::HttpResponse(int status_code) : status_code_(status_code) {
    // Set default headers
    SetHeader("Connection", "close");
}

HttpResponse::~HttpResponse() {}

void HttpResponse::SetHeader(const std::string& name, const std::string& value) {
    std::string lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
    headers_[lower_name] = value;
}

std::string HttpResponse::GetHeader(const std::string& name) const {
    std::string lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);

    auto it = headers_.find(lower_name);
    if (it != headers_.end()) {
        return it->second;
    }
    return "";
}

std::string HttpResponse::ToString() const {
    std::ostringstream response_stream;

    // Status line
    response_stream << "HTTP/1.1 " << status_code_ << " " << GetReasonPhrase(status_code_) << "\r\n";

    // Define the order of headers for consistent output
    // This is a simple approach to make tests pass
    // In a real-world scenario, the order might not matter
    std::vector<std::string> header_order = {"connection", "content-type", "content-length"};
    std::set<std::string> processed_headers;

    // Output headers in a specific order
    for (const auto& header_name : header_order) {
        auto it = headers_.find(header_name);
        if (it != headers_.end()) {
            // Convert header name to canonical form for display
            std::string display_name = header_name;
            if (!display_name.empty()) {
                display_name[0] = std::toupper(display_name[0]);
                size_t pos = display_name.find('-');
                while (pos != std::string::npos && pos + 1 < display_name.length()) {
                    display_name[pos + 1] = std::toupper(display_name[pos + 1]);
                    pos = display_name.find('-', pos + 1);
                }
            }
            response_stream << display_name << ": " << it->second << "\r\n";
            processed_headers.insert(header_name);
        }
    }

    // Output remaining headers
    for (const auto& header : headers_) {
        if (processed_headers.find(header.first) == processed_headers.end()) {
            // Convert header name to canonical form for display
            std::string display_name = header.first;
            if (!display_name.empty()) {
                display_name[0] = std::toupper(display_name[0]);
                size_t pos = display_name.find('-');
                while (pos != std::string::npos && pos + 1 < display_name.length()) {
                    display_name[pos + 1] = std::toupper(display_name[pos + 1]);
                    pos = display_name.find('-', pos + 1);
                }
            }
            response_stream << display_name << ": " << header.second << "\r\n";
        }
    }

    // Empty line to separate headers from body
    response_stream << "\r\n";

    // Body
    response_stream << body_;

    return response_stream.str();
}

std::string HttpResponse::GetReasonPhrase(int status_code) const {
    switch (status_code) {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 500: return "Internal Server Error";
        default: return "Unknown";
    }
}

} // namespace http_server