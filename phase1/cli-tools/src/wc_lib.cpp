/*
 * wc_lib.cpp - Implementation of word count functionality
 *
 * This file contains the core implementation of the word count feature
 * used by the my_wc command-line tool.
 *
 * How to Run with Docker:
 *   1. Build and run my_wc in Docker container:
 *      ./scripts/docker-dev.sh run-wc
 *
 *   2. Or, to build and run manually in Docker:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase1/cli-tools/my_wc [options] [file]
 *
 * How to Run without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *   3. ./build/phase1/cli-tools/my_wc [options] [file]
 */

#include "wc.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

WcResult wc_file(const std::filesystem::path& filepath, std::ostream& out) {
    // Check if file exists
    if (!std::filesystem::exists(filepath)) {
        out << "Error: File does not exist: " << filepath << std::endl;
        return WcResult{};
    }

    // Open file
    std::ifstream file(filepath);
    if (!file.is_open()) {
        out << "Error: Could not open file: " << filepath << std::endl;
        return WcResult{};
    }

    // Read file content
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    // Count lines, words, and characters
    return wc_text(content);
}

WcResult wc_text(const std::string& text) {
    WcResult result;

    // Count characters
    result.characters = text.length();

    // Count lines
    result.lines = std::count(text.begin(), text.end(), '
');
    // If the text doesn't end with a newline, we need to add one more line
    if (!text.empty() && text.back() != '
') {
        result.lines++;
    }

    // Count words
    std::istringstream stream(text);
    std::string word;
    while (stream >> word) {
        result.words++;
    }

    return result;
}
