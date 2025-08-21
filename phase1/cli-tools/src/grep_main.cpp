/*
 * my_grep: A simplified version of the 'grep' command.
 *
 * How to Compile and Run with Docker:
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
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase1/cli-tools/my_grep [pattern] [file]
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

int main(int argc, char* argv[]) {
    bool show_line_numbers = false;
    int pattern_arg_index = 1;
    int file_arg_index = 2;

    // 解析命令行参数
    if (argc > 1 && std::string(argv[1]) == "-n") {
        show_line_numbers = true;
        pattern_arg_index = 2;
        file_arg_index = 3;
    }

    // 检查参数数量
    if (argc < (file_arg_index + 1)) {
        std::cerr << "Usage: " << argv[0] << " [-n] pattern file" << std::endl;
        return 1;
    }

    std::string pattern = argv[pattern_arg_index];
    std::filesystem::path filepath = argv[file_arg_index];

    // 调用grep函数
    bool success = grep_file(pattern, filepath, std::cout, show_line_numbers);

    return success ? 0 : 1;
}
