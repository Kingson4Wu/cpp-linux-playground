#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <string>
#include <functional>
#include <curl/curl.h> // Include curl.h for curl_off_t and CURL

namespace threaded_downloader {

/**
 * @brief Class responsible for downloading a single file.
 * 
 * This class encapsulates the logic for downloading a file from a given URL
 * to a specified local path using libcurl.
 */
class Downloader {
public:
    /**
     * @brief Type alias for the progress callback function.
     * 
     * The callback function takes three parameters:
     * - The URL being downloaded.
     * - The number of bytes downloaded so far.
     * - The total size of the file (if known, otherwise -1).
     */
    using ProgressCallback = std::function<void(const std::string&, long long, long long)>;

    /**
     * @brief Constructor for Downloader.
     * 
     * @param url The URL of the file to download.
     * @param filepath The local path where the file should be saved.
     * @param progress_callback An optional callback function to report progress.
     * @param resume Whether to resume a partial download if the file exists.
     * @param speed_limit The maximum download speed in bytes per second (0 for no limit).
     */
    Downloader(const std::string& url, const std::string& filepath, 
               ProgressCallback progress_callback = nullptr,
               bool resume = false, long speed_limit = 0);

    /**
     * @brief Destructor for Downloader.
     * 
     * Cleans up any resources used by libcurl.
     */
    ~Downloader();

    /**
     * @brief Starts the download process.
     * 
     * This method performs the actual HTTP request and writes the data to the file.
     * It blocks until the download is complete or an error occurs.
     * 
     * @return True if the download was successful, false otherwise.
     */
    bool Download();

private:
    std::string url_;
    std::string filepath_;
    ProgressCallback progress_callback_;
    bool resume_;
    long speed_limit_;
    CURL* curl_;

    // Disable copy constructor and assignment operator
    Downloader(const Downloader&) = delete;
    Downloader& operator=(const Downloader&) = delete;

    // Friend function for the C-style callback
    friend int ProgressCallbackWrapper(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
};

} // namespace threaded_downloader

#endif // DOWNLOADER_H
