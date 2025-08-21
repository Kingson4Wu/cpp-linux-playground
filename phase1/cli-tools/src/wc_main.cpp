/*
 * my_wc: A simplified version of the 'wc' command.
 *
 * How to Compile and Run with Docker:
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
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase1/cli-tools/my_wc [options] [file]
 *
 * Usage Examples:
 *   - Count lines, words, and characters in a file:
 *     ./build/phase1/cli-tools/my_wc file.txt
 *
 *   - Count only lines in a file:
 *     ./build/phase1/cli-tools/my_wc -l file.txt
 *
 *   - Count only words in a file:
 *     ./build/phase1/cli-tools/my_wc -w file.txt
 *
 *   - Count only characters in a file:
 *     ./build/phase1/cli-tools/my_wc -c file.txt
 *
 *   - Count from standard input:
 *     echo "Hello world" | ./build/phase1/cli-tools/my_wc
 * 
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "my_wc") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "wc.h"
#include <iostream>
#include <filesystem>
#include <string>

int main(int argc, char* argv[]) {
    bool count_lines = false;
    bool count_words = false;
    bool count_chars = false;
    std::string filepath;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-l") {
            count_lines = true;
        } else if (arg == "-w") {
            count_words = true;
        } else if (arg == "-c") {
            count_chars = true;
        } else if (filepath.empty()) {
            filepath = arg;
        } else {
            std::cerr << "Usage: " << argv[0] << " [-lwc] [file]" << std::endl;
            return 1;
        }
    }

    // If no options are specified, enable all counts
    if (!count_lines && !count_words && !count_chars) {
        count_lines = count_words = count_chars = true;
    }

    WcResult result;

    // If no file is specified, read from standard input
    if (filepath.empty()) {
        std::string content((std::istreambuf_iterator<char>(std::cin)),
                            std::istreambuf_iterator<char>());
        result = wc_text(content);
    } else {
        // Count from file
        result = wc_file(filepath, std::cerr);
        // If there was an error, exit with error code
        if (result.lines == 0 && result.words == 0 && result.characters == 0) {
            return 1;
        }
    }

    // Print results based on selected options
    bool needs_space = false;
    if (count_lines) {
        if (needs_space) std::cout << " ";
        std::cout << result.lines;
        needs_space = true;
    }
    if (count_words) {
        if (needs_space) std::cout << " ";
        std::cout << result.words;
        needs_space = true;
    }
    if (count_chars) {
        if (needs_space) std::cout << " ";
        std::cout << result.characters;
        needs_space = true;
    }

    // Print filename if a file was specified
    if (!filepath.empty()) {
        std::cout << " " << filepath;
    }

    std::cout << std::endl;

    return 0;
}
