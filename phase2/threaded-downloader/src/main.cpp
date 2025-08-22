/**
 * @file main.cpp
 * @brief Main entry point for the multi-threaded downloader application.
 *
 * This application allows users to download multiple files concurrently from the command line.
 * It uses libcurl for HTTP requests and C++ standard library threads for concurrency.
 *
 * How to Compile and Run with Docker:
 *   1. Build and run my_downloader in Docker container:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase2/threaded-downloader/my_downloader [OPTIONS] <URL1> <URL2> ...
 *
 *   2. Or, to build and run manually in Docker:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase2/threaded-downloader/my_downloader [OPTIONS] <URL1> <URL2> ...
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase2/threaded-downloader/my_downloader [OPTIONS] <URL1> <URL2> ...
 *
 * Usage:
 *   ./build/phase2/threaded-downloader/my_downloader [OPTIONS] <URL1> <URL2> ...
 *
 * Options:
 *   -j, --jobs N       Number of concurrent downloads (default: 4)
 *   -o, --output DIR   Output directory for downloaded files (default: current directory)
 *   -h, --help         Show this help message
 *
 * Examples:
 *   # Download two files with default settings
 *   ./build/phase2/threaded-downloader/my_downloader http://example.com/file1.zip http://example.com/file2.pdf
 *
 *   # Download with 8 concurrent connections and save to /tmp/downloads
 *   ./build/phase2/threaded-downloader/my_downloader -j 8 -o /tmp/downloads http://example.com/file1.zip http://example.com/file2.pdf
 *   
 *   # Real Example   
 *   ./build/phase2/threaded-downloader/my_downloader -j 8 -o .  https://dldir1.qq.com/qqfile/qq/PCQQ9.7.17/QQ9.7.17.29225.exe https://wirelesscdn-download.xuexi.cn/publish/xuexi_android/latest/xuexi_android_10002068.apk
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "my_downloader") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "download_manager.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <getopt.h> // For getopt_long
#include <filesystem> // To handle path operations

// Function to print help message
void PrintHelp(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [OPTIONS] <URL1> <URL2> ...
"
              << "Download files from URLs concurrently.

"
              << "Options:
"
              << "  -j, --jobs N       Number of concurrent downloads (default: 4)
"
              << "  -o, --output DIR   Output directory for downloaded files (default: current directory)
"
              << "  -h, --help         Show this help message
";
}

int main(int argc, char* argv[]) {
    // Default values
    size_t max_concurrent_downloads = 4;
    std::filesystem::path output_dir = "."; // Current directory

    // Define long options
    static struct option long_options[] = {
        {"jobs", required_argument, 0, 'j'},
        {"output", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "j:o:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'j':
                try {
                    max_concurrent_downloads = std::stoull(optarg);
                    if (max_concurrent_downloads == 0) {
                        std::cerr << "Error: Number of jobs must be greater than 0." << std::endl;
                        return 1;
                    }
                } catch (const std::exception&) {
                    std::cerr << "Error: Invalid number of jobs specified." << std::endl;
                    return 1;
                }
                break;
            case 'o':
                output_dir = optarg;
                // Validate output directory
                if (!std::filesystem::exists(output_dir)) {
                    std::cerr << "Error: Output directory '" << output_dir << "' does not exist." << std::endl;
                    return 1;
                }
                if (!std::filesystem::is_directory(output_dir)) {
                    std::cerr << "Error: '" << output_dir << "' is not a directory." << std::endl;
                    return 1;
                }
                break;
            case 'h':
                PrintHelp(argv[0]);
                return 0;
            default:
                std::cerr << "Unknown option. Use -h for help." << std::endl;
                return 1;
        }
    }

    // Collect URLs from remaining arguments
    std::vector<std::string> urls;
    for (int i = optind; i < argc; ++i) {
        urls.emplace_back(argv[i]);
    }

    if (urls.empty()) {
        std::cerr << "Error: No URLs provided.
";
        PrintHelp(argv[0]);
        return 1;
    }

    // Initialize CURL globally
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Create DownloadManager
    threaded_downloader::DownloadManager manager(max_concurrent_downloads);

    // Add download tasks
    for (const auto& url : urls) {
        std::string filename = threaded_downloader::utils::GetFileNameFromUrl(url);
        std::filesystem::path full_filepath = output_dir / filename;
        std::string filepath_str = full_filepath.string();

        if (!manager.AddDownload(url, filepath_str)) {
            std::cerr << "Failed to add download for URL: " << url << std::endl;
            // Depending on requirements, you might want to continue or abort
            // For now, let's continue
        }
    }

    // Wait for all downloads to complete
    manager.Wait();

    // Cleanup CURL globally
    curl_global_cleanup();

    std::cout << "All downloads completed." << std::endl;
    return 0;
}
