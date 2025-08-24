#ifndef DOWNLOAD_MANAGER_H
#define DOWNLOAD_MANAGER_H

#include "downloader.h"
#include <vector>
#include <string>
#include <future>
#include <memory>

namespace threaded_downloader {

/**
 * @brief Class responsible for managing multiple concurrent downloads.
 * 
 * This class manages a queue of download tasks and executes them using a 
 * thread pool to achieve concurrency.
 */
class DownloadManager {
public:
    /**
     * @brief Constructor for DownloadManager.
     * 
     * @param max_concurrent_downloads The maximum number of downloads to run concurrently.
     * @param resume Whether to resume partial downloads.
     * @param speed_limit The maximum download speed in bytes per second (0 for no limit).
     */
    explicit DownloadManager(size_t max_concurrent_downloads = 4, bool resume = false, long speed_limit = 0);

    /**
     * @brief Destructor for DownloadManager.
     * 
     * Waits for all ongoing downloads to complete before destruction.
     */
    ~DownloadManager();

    /**
     * @brief Adds a download task to the manager.
     * 
     * @param url The URL of the file to download.
     * @param filepath The local path where the file should be saved.
     * @return True if the task was added successfully, false if the manager is shutting down.
     */
    bool AddDownload(const std::string& url, const std::string& filepath);

    /**
     * @brief Waits for all added downloads to complete.
     * 
     * This method blocks until all download tasks that were added via AddDownload
     * have finished (either successfully or with an error).
     */
    void Wait();

private:
    size_t max_concurrent_downloads_;
    bool resume_;
    long speed_limit_;
    std::vector<std::future<bool>> futures_;

    // A simple flag to indicate if the manager is shutting down.
    // In a more complex scenario, a proper synchronization mechanism would be needed.
    bool shutdown_;

    // Disable copy constructor and assignment operator
    DownloadManager(const DownloadManager&) = delete;
    DownloadManager& operator=(const DownloadManager&) = delete;

    /**
     * @brief Helper function to create and run a download task.
     * 
     * @param url The URL of the file to download.
     * @param filepath The local path where the file should be saved.
     * @param resume Whether to resume a partial download.
     * @param speed_limit The maximum download speed in bytes per second.
     * @return The result of the download operation.
     */
    static bool RunDownload(const std::string& url, const std::string& filepath, bool resume, long speed_limit);
};

} // namespace threaded_downloader

#endif // DOWNLOAD_MANAGER_H
