/*
 * my_ls: A simplified version of the 'ls' command.
 *
 * How to Compile and Run with Docker:
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
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase1/cli-tools/my_ls [path]
 *
 * Usage Examples:
 *   - List contents of the current directory:
 *     ./build/phase1/cli-tools/my_ls
 *
 *   - List contents of a specific directory (e.g., 'phase1'):
 *     ./build/phase1/cli-tools/my_ls phase1
 * 
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "my_ls") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
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
