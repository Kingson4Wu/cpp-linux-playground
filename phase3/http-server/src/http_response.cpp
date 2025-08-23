#include "http_response.h"
#include <sstream>
#include <algorithm>
#include <cctype>

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
    response_stream << "HTTP/1.1 " << status_code_ << " " << GetReasonPhrase(status_code_) << "\r
";

    // Headers
    for (const auto& header : headers_) {
        response_stream << header.first << ": " << header.second << "\r
";
    }

    // Empty line to separate headers from body
    response_stream << "\r
";

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
