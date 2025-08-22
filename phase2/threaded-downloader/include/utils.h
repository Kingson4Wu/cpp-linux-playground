#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace threaded_downloader {
namespace utils {

/**
 * @brief Extracts the filename from a URL.
 * 
 * This function parses the URL and returns the last component after the final '/'.
 * If the URL ends with a '/', or if there is no '/' in the path, it returns "index.html".
 * 
 * @param url The URL string.
 * @return The extracted filename.
 */
std::string GetFileNameFromUrl(const std::string& url);

/**
 * @brief Creates the necessary directories for a given file path.
 * 
 * This function ensures that all parent directories of the specified file path exist.
 * If they don't exist, they are created.
 * 
 * @param filepath The full path to the file.
 * @return True if the directories were created successfully or already existed, false otherwise.
 */
bool CreateDirectories(const std::string& filepath);

} // namespace utils
} // namespace threaded_downloader

#endif // UTILS_H
