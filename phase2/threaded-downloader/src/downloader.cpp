#include "downloader.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <cstring> // For strerror
#include <cerrno>  // For errno

namespace threaded_downloader {

// Callback function for writing received data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::ofstream* file) {
    size_t total_size = size * nmemb;
    if (file) {
        file->write(static_cast<const char*>(contents), total_size);
        if (file->fail()) {
            std::cerr << "Error writing to file." << std::endl;
            return 0; // Signal an error to libcurl
        }
        return total_size;
    }
    return 0;
}

// Wrapper function for progress reporting to access private members
int ProgressCallbackWrapper(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    Downloader* downloader = static_cast<Downloader*>(clientp);
    if (downloader && downloader->progress_callback_) {
        // Cast to long long to match the callback signature
        downloader->progress_callback_(downloader->url_, static_cast<long long>(dlnow), static_cast<long long>(dltotal));
    }
    return 0; // Returning a non-zero value will abort the transfer
}

Downloader::Downloader(const std::string& url, const std::string& filepath, ProgressCallback progress_callback, bool resume, long speed_limit)
    : url_(url), filepath_(filepath), progress_callback_(progress_callback), resume_(resume), speed_limit_(speed_limit), curl_(nullptr) {
    curl_ = curl_easy_init();
}

Downloader::~Downloader() {
    if (curl_) {
        curl_easy_cleanup(curl_);
    }
}

bool Downloader::Download() {
    if (!curl_) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return false;
    }

    // Open file for writing (binary mode)
    std::ios_base::openmode mode = std::ios::binary;
    if (resume_) {
        // Check if file exists and get its size
        std::ifstream existing_file(filepath_, std::ios::binary | std::ios::ate);
        if (existing_file.is_open()) {
            // File exists, open in append mode
            mode |= std::ios::app;
        }
    }
    
    std::ofstream file(filepath_, mode);
    if (!file.is_open()) {
        std::cerr << "Failed to open file '" << filepath_ << "' for writing: " << strerror(errno) << std::endl;
        return false;
    }

    // If resuming, get the current file size to set the range header
    curl_off_t resume_from = 0;
    if (resume_) {
        file.seekp(0, std::ios::end);
        resume_from = file.tellp();
    }

    // Set CURL options
    curl_easy_setopt(curl_, CURLOPT_URL, url_.c_str());
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &file);

    // Enable progress reporting
    curl_easy_setopt(curl_, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl_, CURLOPT_XFERINFOFUNCTION, ProgressCallbackWrapper);
    curl_easy_setopt(curl_, CURLOPT_XFERINFODATA, this);

    // Follow HTTP redirects
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);

    // Fail on HTTP error codes (e.g., 404)
    curl_easy_setopt(curl_, CURLOPT_FAILONERROR, 1L);

    // Set resume position if needed
    if (resume_ && resume_from > 0) {
        curl_easy_setopt(curl_, CURLOPT_RESUME_FROM_LARGE, resume_from);
    }

    // Set speed limit if specified
    if (speed_limit_ > 0) {
        curl_easy_setopt(curl_, CURLOPT_MAX_RECV_SPEED_LARGE, speed_limit_);
    }

    // Perform the request
    CURLcode res = curl_easy_perform(curl_);

    // Check for errors
    if (res != CURLE_OK) {
        std::cerr << "Download failed for '" << url_ << "': " << curl_easy_strerror(res) << std::endl;
        file.close();
        // If not resuming, attempt to remove the partially downloaded file
        if (!resume_ && std::remove(filepath_.c_str()) != 0) {
            std::cerr << "Warning: Failed to remove incomplete file '" << filepath_ << "': " << strerror(errno) << std::endl;
        }
        return false;
    }

    // Get HTTP response code
    long response_code;
    curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code >= 400) {
        std::cerr << "HTTP Error " << response_code << " for '" << url_ << "'" << std::endl;
        file.close();
        if (!resume_ && std::remove(filepath_.c_str()) != 0) {
            std::cerr << "Warning: Failed to remove incomplete file '" << filepath_ << "': " << strerror(errno) << std::endl;
        }
        return false;
    }

    file.close();
    std::cout << "Downloaded '" << url_ << "' to '" << filepath_ << "'" << std::endl;
    return true;
}

} // namespace threaded_downloader