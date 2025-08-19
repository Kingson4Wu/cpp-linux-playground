/*
 * my_ls: A simplified version of the 'ls' command.
 *
 * How to Compile:
 *   (From the project root directory)
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run:
 *   ./build/phase1/cli-tools/my_ls [path]
 *
 * Usage Examples:
 *   - List contents of the current directory:
 *     ./build/phase1/cli-tools/my_ls
 *
 *   - List contents of a specific directory (e.g., 'phase1'):
 *     ./build/phase1/cli-tools/my_ls phase1
 */

#include "ls.h"
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " [path]" << std::endl;
        return 1;
    }

    std::filesystem::path current_path = ".";
    if (argc == 2) {
        current_path = argv[1];
    }

    list_directory(current_path, std::cout);

    return 0;
}
