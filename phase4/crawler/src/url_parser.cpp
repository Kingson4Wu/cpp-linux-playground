#include "url_parser.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace crawler {

UrlParser::UrlParser(const std::string& url) : port_(0), valid_(false) {
    Parse(url);
}

const std::string& UrlParser::GetProtocol() const {
    return protocol_;
}

const std::string& UrlParser::GetHost() const {
    return host_;
}

int UrlParser::GetPort() const {
    return port_;
}

const std::string& UrlParser::GetPath() const {
    return path_;
}

const std::string& UrlParser::GetQuery() const {
    return query_;
}

const std::string& UrlParser::GetFragment() const {
    return fragment_;
}

bool UrlParser::IsValid() const {
    return valid_;
}

void UrlParser::Parse(const std::string& url) {
    // Reset all fields
    protocol_.clear();
    host_.clear();
    port_ = 0;
    path_.clear();
    query_.clear();
    fragment_.clear();
    valid_ = false;

    if (url.empty()) {
        return;
    }

    std::string::size_type pos = 0;
    
    // Parse protocol
    std::string::size_type protocol_end = url.find("://");
    if (protocol_end != std::string::npos) {
        protocol_ = url.substr(0, protocol_end);
        // Convert to lowercase
        std::transform(protocol_.begin(), protocol_.end(), protocol_.begin(), 
                       [](unsigned char c) { return std::tolower(c); });
        pos = protocol_end + 3;
    } else {
        // Default to http
        protocol_ = "http";
    }
    
    // Set default port based on protocol
    if (protocol_ == "https") {
        port_ = 443;
    } else {
        port_ = 80;
    }
    
    // Parse host
    std::string::size_type host_end = url.find('/', pos);
    std::string::size_type port_pos = url.find(':', pos);
    
    // Check if port is specified
    if (port_pos != std::string::npos && (host_end == std::string::npos || port_pos < host_end)) {
        host_ = url.substr(pos, port_pos - pos);
        port_pos++; // Skip ':'
        
        // Parse port
        std::string port_str;
        if (host_end != std::string::npos) {
            port_str = url.substr(port_pos, host_end - port_pos);
        } else {
            port_str = url.substr(port_pos);
        }
        
        try {
            port_ = std::stoi(port_str);
        } catch (const std::exception&) {
            return; // Invalid port
        }
    } else {
        if (host_end != std::string::npos) {
            host_ = url.substr(pos, host_end - pos);
        } else {
            host_ = url.substr(pos);
        }
    }
    
    if (host_.empty()) {
        return;
    }
    
    // Move position past host
    if (host_end != std::string::npos) {
        pos = host_end;
    } else {
        pos = url.length();
    }
    
    // Parse path
    std::string::size_type query_pos = url.find('?', pos);
    std::string::size_type fragment_pos = url.find('#', pos);
    
    if (query_pos != std::string::npos) {
        path_ = url.substr(pos, query_pos - pos);
    } else if (fragment_pos != std::string::npos) {
        path_ = url.substr(pos, fragment_pos - pos);
    } else {
        path_ = url.substr(pos);
    }
    
    // Default path
    if (path_.empty()) {
        path_ = "/";
    }
    
    // Parse query
    if (query_pos != std::string::npos) {
        std::string::size_type query_end = fragment_pos;
        if (query_end != std::string::npos) {
            query_ = url.substr(query_pos + 1, query_end - query_pos - 1);
        } else {
            query_ = url.substr(query_pos + 1);
        }
    }
    
    // Parse fragment
    if (fragment_pos != std::string::npos) {
        fragment_ = url.substr(fragment_pos + 1);
    }
    
    valid_ = true;
}

} // namespace crawler