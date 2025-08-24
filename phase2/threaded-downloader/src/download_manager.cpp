#include "download_manager.h"
#include "utils.h"
#include <iostream>
#include <algorithm> // For std::min>

namespace threaded_downloader {

DownloadManager::DownloadManager(size_t max_concurrent_downloads, bool resume, long speed_limit)
    : max_concurrent_downloads_(max_concurrent_downloads), resume_(resume), speed_limit_(speed_limit), shutdown_(false) {}

DownloadManager::~DownloadManager() {
    Wait();
}

bool DownloadManager::AddDownload(const std::string& url, const std::string& filepath) {
    if (shutdown_) {
        std::cerr << "DownloadManager is shutting down, cannot add new downloads." << std::endl;
        return false;
    }

    // Ensure parent directories exist
    if (!utils::CreateDirectories(filepath)) {
        std::cerr << "Failed to create directories for '" << filepath << "'" << std::endl;
        return false;
    }

    // Add the download task to the future list
    futures_.emplace_back(std::async(std::launch::async, RunDownload, url, filepath, resume_, speed_limit_));

    // If we've reached the max concurrent downloads, wait for some to finish
    // This is a simple way to limit concurrency. A more sophisticated approach
    // would use a thread pool.
    if (futures_.size() >= max_concurrent_downloads_) {
        // Wait for the oldest future to complete
        if (!futures_.empty()) {
            futures_.front().wait();
            // Remove completed futures from the list
            futures_.erase(futures_.begin());
        }
    }

    return true;
}

void DownloadManager::Wait() {
    shutdown_ = true;
    // Wait for all remaining futures to complete
    for (auto& future : futures_) {
        future.wait();
        // Optionally, you could check the result here
        // bool success = future.get();
    }
    futures_.clear();
}

bool DownloadManager::RunDownload(const std::string& url, const std::string& filepath, bool resume, long speed_limit) {
    // Simple progress callback for demonstration
    Downloader::ProgressCallback progress_cb = [](const std::string& url, long long dlnow, long long dltotal) {
        if (dltotal > 0) {
            int percent = static_cast<int>((100.0 * dlnow) / dltotal);
            std::cout << "Downloading " << url << ": " << percent << "% (" << dlnow << "/" << dltotal << " bytes)\r" << std::flush;
        } else {
            std::cout << "Downloading " << url << ": " << dlnow << " bytes\r" << std::flush;
        }
    };

    Downloader downloader(url, filepath, progress_cb, resume, speed_limit);
    bool success = downloader.Download();

    // Print a newline after progress to avoid overwriting
    std::cout << std::endl;

    if (!success) {
        std::cerr << "Download failed for '" << url << "'" << std::endl;
    }

    return success;
}

} // namespace threaded_downloader