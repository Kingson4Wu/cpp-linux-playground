/*
 * grep_lib.cpp - Implementation of grep functionality
 *
 * This file contains the core implementation of the grep feature
 * used by the my_grep command-line tool.
 *
 * How to Run with Docker:
 *   1. Build and run my_grep in Docker container:
 *      ./scripts/docker-dev.sh run-grep
 *
 *   2. Or, to build and run manually in Docker:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase1/cli-tools/my_grep [pattern] [file]
 *
 * How to Run without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *   3. ./build/phase1/cli-tools/my_grep [pattern] [file]
 */

#include "grep.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

bool grep_file(const std::string& pattern, const std::filesystem::path& filepath, 
               std::ostream& out, bool show_line_numbers) {
    // Check if file exists
    if (!std::filesystem::exists(filepath)) {
        out << "Error: File does not exist: " << filepath << std::endl;
        return false;
    }

    // Open file
    std::ifstream file(filepath);
    if (!file.is_open()) {
        out << "Error: Could not open file: " << filepath << std::endl;
        return false;
    }

    // Read file and search for pattern
    std::string line;
    int line_number = 0;

    while (std::getline(file, line)) {
        line_number++;

        // Check if line contains pattern
        if (line.find(pattern) != std::string::npos) {
            if (show_line_numbers) {
                out << line_number << ":";
            }
            out << line << std::endl;
        }
    }

    return true;
}

void grep_text(const std::string& pattern, const std::string& text, 
               std::ostream& out, bool show_line_numbers) {
    std::istringstream stream(text);
    std::string line;
    int line_number = 0;

    while (std::getline(stream, line)) {
        line_number++;

        // Check if line contains pattern
        if (line.find(pattern) != std::string::npos) {
            if (show_line_numbers) {
                out << line_number << ":";
            }
            out << line << std::endl;
        }
    }
}
