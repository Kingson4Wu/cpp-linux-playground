/*
 * my_grep: A simplified version of the 'grep' command.
 *
 * How to Run with Docker (builds and runs automatically):
 *   ./scripts/docker-dev.sh run-grep [-n] pattern file
 *
 * How to Compile and Run manually in Docker:
 *   1. Enter the Docker container:
 *      ./scripts/docker-dev.sh exec
 *   2. Inside the container, create a build directory and compile:
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *   3. Run the executable:
 *      ./phase1/cli-tools/my_grep [-n] pattern file
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase1/cli-tools/my_grep [-n] pattern file
 *
 * Usage Examples:
 *   - Search for a pattern in a file:
 *     ./build/phase1/cli-tools/my_grep "pattern" file.txt
 *
 *   - Search for a pattern in a file with line numbers:
 *     ./build/phase1/cli-tools/my_grep -n "pattern" file.txt
 * 
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "my_grep") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "grep.h"
#include <iostream>
#include <filesystem>

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [-n] pattern file\n"
              << "  -n\tShow line numbers\n"
              << "  -h\tDisplay this help message\n";
}

int main(int argc, char* argv[]) {
    bool show_line_numbers = false;
    int pattern_arg_index = 1;
    int file_arg_index = 2;

    // Parse command line arguments
    if (argc > 1) {
        std::string arg1 = argv[1];
        if (arg1 == "-n") {
            show_line_numbers = true;
            pattern_arg_index = 2;
            file_arg_index = 3;
        } else if (arg1 == "-h" || arg1 == "--help") {
            print_usage(argv[0]);
            return 0;
        }
    }

    // Check argument count
    if (argc < (file_arg_index + 1)) {
        std::cerr << "Usage: " << argv[0] << " [-n] pattern file" << std::endl;
        return 1;
    }

    std::string pattern = argv[pattern_arg_index];
    std::filesystem::path filepath = argv[file_arg_index];

    // Call grep function
    bool success = grep_file(pattern, filepath, std::cout, show_line_numbers);

    return success ? 0 : 1;
}
