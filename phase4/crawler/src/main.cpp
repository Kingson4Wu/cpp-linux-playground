/**
 * @file main.cpp
 * @brief Main entry point for the high-concurrency web crawler.
 *
 * This application implements a high-concurrency web crawler that can
 * fetch web pages concurrently using a thread pool.
 *
 * How to Compile and Run:
 *   1. From the project root directory:
 *      cmake -S . -B build
 *      cmake --build build -- -j
 *   2. Run the crawler:
 *      ./build/phase4/crawler/crawler [URLs...]
 *
 * Usage:
 *   ./build/phase4/crawler/crawler [URLs...]
 *
 * Examples:
 *   # Crawl a single URL
 *   ./build/phase4/crawler/crawler http://example.com
 *
 *   # Crawl multiple URLs
 *   ./build/phase4/crawler/crawler http://example.com http://google.com
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "crawler") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "crawler.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [URLs...]" << std::endl;
        std::cerr << "Example: " << argv[0] << " http://example.com http://google.com" << std::endl;
        return 1;
    }
    
    // Create crawler with 4 threads and max 100 pages
    crawler::Crawler crawler(4, 100);
    
    // Add URLs to crawl
    for (int i = 1; i < argc; ++i) {
        std::string url = argv[i];
        std::cout << "Adding URL to crawl: " << url << std::endl;
        crawler.AddUrl(url);
    }
    
    // Create output directory
    std::filesystem::create_directories("crawled_pages");
    
    // Start crawling with a callback to save pages
    crawler.Start([](const std::string& url, const std::string& content) {
        std::cout << "Crawled: " << url << " (size: " << content.size() << " bytes)" << std::endl;
        
        // Save content to file
        std::string filename = "crawled_pages/" + std::to_string(std::hash<std::string>{}(url)) + ".html";
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "Saved to: " << filename << std::endl;
        } else {
            std::cerr << "Failed to save: " << filename << std::endl;
        }
    });
    
    // Wait for crawling to complete
    crawler.Wait();
    
    std::cout << "Crawling completed. Crawled " << crawler.GetCrawledCount() << " pages." << std::endl;
    
    return 0;
}