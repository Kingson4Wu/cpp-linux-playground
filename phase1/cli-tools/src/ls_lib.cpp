/*
 * ls_lib.cpp - Implementation of directory listing functionality
 *
 * This file contains the core implementation of the directory listing feature
 * used by the my_ls command-line tool.
 *
 * How to Run with Docker:
 *   1. Build and run my_ls in Docker container:
 *      ./scripts/docker-dev.sh run-ls
 *
 *   2. Or, to build and run manually in Docker:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase1/cli-tools/my_ls [path]
 *
 * How to Run without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *   3. ./build/phase1/cli-tools/my_ls [path]
 */

#include "ls.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

void list_directory(const std::filesystem::path& path, std::ostream& out) {
    try {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("Error: Path does not exist.");
        }

        if (!std::filesystem::is_directory(path)) {
            // If it's a file, just print its name
            out << path.filename().string() << std::endl;
            return;
        }

        std::vector<std::string> entries;
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            entries.push_back(entry.path().filename().string());
        }

        std::sort(entries.begin(), entries.end());

        for (const auto& entry_name : entries) {
            out << entry_name << std::endl;
        }

    } catch (const std::exception& e) {
        // In a real-world scenario, we might want to output errors to std::cerr
        // but for testability of this function's output, we'll send it to the stream.
        out << e.what() << std::endl;
    }
}
