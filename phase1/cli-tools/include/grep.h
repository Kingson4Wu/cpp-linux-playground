#pragma once

#include <filesystem>
#include <iosfwd>
#include <string>
#include <vector>

/**
 * @brief Search for matching lines in a file
 * 
 * @param pattern Search pattern
 * @param filepath File path
 * @param out Output stream
 * @param show_line_numbers Whether to show line numbers
 * @return true Search successful
 * @return false Search failed (e.g., file does not exist)
 */
bool grep_file(const std::string& pattern, const std::filesystem::path& filepath, 
               std::ostream& out, bool show_line_numbers = false);

/**
 * @brief Search for matching lines in text
 * 
 * @param pattern Search pattern
 * @param text Input text
 * @param out Output stream
 * @param show_line_numbers Whether to show line numbers
 */
void grep_text(const std::string& pattern, const std::string& text, 
               std::ostream& out, bool show_line_numbers = false);
