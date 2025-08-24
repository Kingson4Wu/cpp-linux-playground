#pragma once

#include <filesystem>
#include <iosfwd>

/**
 * @brief Structure to hold the results of word count operations
 */
struct WcResult {
    int lines = 0;
    int words = 0;
    int characters = 0;
    bool success = true;  // Indicates if the operation was successful
};

/**
 * @brief Count lines, words, and characters in a file
 * 
 * @param filepath Path to the file to count
 * @param out Output stream for error messages
 * @return WcResult Structure containing the counts, or zeros if an error occurred
 */
WcResult wc_file(const std::filesystem::path& filepath, std::ostream& out);

/**
 * @brief Count lines, words, and characters in text
 * 
 * @param text Input text to count
 * @return WcResult Structure containing the counts
 */
WcResult wc_text(const std::string& text);
