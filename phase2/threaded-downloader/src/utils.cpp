#include "utils.h"
#include <filesystem>
#include <iostream>
#include <algorithm> // For std::find

namespace threaded_downloader {
namespace utils {

std::string GetFileNameFromUrl(const std::string& url) {
    // Find the last occurrence of '/'
    size_t last_slash_pos = url.find_last_of('/');

    // If '/' is not found, return default
    if (last_slash_pos == std::string::npos) {
        return "index.html";
    }

    // Extract the substring after the last '/'
    std::string filename = url.substr(last_slash_pos + 1);

    // If the extracted name is empty or the URL ends with '/', return default
    if (filename.empty() || url.back() == '/') {
        return "index.html";
    }

    // Special case: if the filename doesn't contain a dot and is the host name, return default
    // This handles cases like "http://example.com" or "https://example.com"
    size_t protocol_end = url.find("://");
    if (protocol_end != std::string::npos) {
        size_t host_start = protocol_end + 3;
        size_t host_end = url.find('/', host_start);
        if (host_end == std::string::npos) {
            host_end = url.length();
        }
        std::string host = url.substr(host_start, host_end - host_start);

        // If the filename is the same as the host, it means there's no path component
        // or the path is just the host, so return default
        if (filename == host) {
            return "index.html";
        }
    }

    return filename;
}

bool CreateDirectories(const std::string& filepath) {
    try {
        // Use std::filesystem to create parent directories
        std::filesystem::path path_obj(filepath);
        std::filesystem::create_directories(path_obj.parent_path());
        return true;
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Filesystem error: " << ex.what() << std::endl;
        return false;
    }
}

} // namespace utils
} // namespace threaded_downloader
