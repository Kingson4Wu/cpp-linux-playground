#include "http_request.h"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace http_server {

HttpRequest::HttpRequest() {}

HttpRequest::~HttpRequest() {}

bool HttpRequest::Parse(const std::string& request_str) {
    std::istringstream request_stream(request_str);
    std::string line;

    // Parse the request line
    if (!std::getline(request_stream, line)) {
        return false; // Empty request
    }

    // Remove trailing 
    if (!line.empty() && line.back() == '
') {
        line.pop_back();
    }

    std::istringstream request_line_stream(line);
    if (!(request_line_stream >> method_ >> uri_ >> version_)) {
        return false; // Malformed request line
    }

    // Convert method to uppercase for consistency
    std::transform(method_.begin(), method_.end(), method_.begin(), ::toupper);

    // Parse headers
    headers_.clear();
    while (std::getline(request_stream, line) && line != "
") {
        // Remove trailing 
        if (!line.empty() && line.back() == '
') {
            line.pop_back();
        }

        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string name = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);

            // Trim leading whitespace from value
            value.erase(0, value.find_first_not_of(' '));

            // Store header (name is case-insensitive)
            std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            headers_[name] = value;
        }
    }

    // Parse body (if any)
    body_.clear();
    std::ostringstream body_stream;
    body_stream << request_stream.rdbuf();
    body_ = body_stream.str();

    return true;
}

std::string HttpRequest::GetHeader(const std::string& name) const {
    std::string lower_name = name;
    std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);

    auto it = headers_.find(lower_name);
    if (it != headers_.end()) {
        return it->second;
    }
    return "";
}

} // namespace http_server
