/**
 * @file main.cpp
 * @brief Main entry point for the mini-search engine.
 *
 * This application implements a simple search engine that can index documents
 * and process search queries using TF-IDF scoring.
 *
 * How to Compile and Run:
 *   1. From the project root directory:
 *      cmake -S . -B build
 *      cmake --build build -- -j
 *   2. Run the search engine:
 *      ./build/phase4/mini-search/mini_search
 *
 * Usage:
 *   ./build/phase4/mini-search/mini_search
 *
 * Examples:
 *   # Run the search engine
 *   ./build/phase4/mini-search/mini_search
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "mini_search") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "search_engine.h"
#include <iostream>
#include <string>
#include <vector>

int main() {
    search::SearchEngine engine;
    
    // Add some sample documents
    std::cout << "Adding sample documents..." << std::endl;
    
    search::Document doc1(1, "The quick brown fox jumps over the lazy dog", "Doc 1");
    search::Document doc2(2, "A quick brown dog jumps over a lazy fox", "Doc 2");
    search::Document doc3(3, "The lazy dog and fox are quick and brown", "Doc 3");
    search::Document doc4(4, "Programming in C++ is fun and challenging", "Doc 4");
    search::Document doc5(5, "C++ is a powerful programming language", "Doc 5");
    
    engine.AddDocument(doc1);
    engine.AddDocument(doc2);
    engine.AddDocument(doc3);
    engine.AddDocument(doc4);
    engine.AddDocument(doc5);
    
    std::cout << "Documents added successfully!" << std::endl;
    
    // Interactive search loop
    std::string query;
    while (true) {
        std::cout << "\nEnter a search query (or 'quit' to exit): ";
        std::getline(std::cin, query);
        
        if (query == "quit") {
            break;
        }
        
        if (query.empty()) {
            continue;
        }
        
        auto results = engine.Search(query, 5);
        
        if (results.empty()) {
            std::cout << "No results found." << std::endl;
        } else {
            std::cout << "Search results:" << std::endl;
            for (const auto& result : results) {
                int doc_id = result.first;
                double score = result.second;
                
                const search::Document* doc = engine.GetDocument(doc_id);
                if (doc) {
                    std::cout << "Document " << doc_id << " (Score: " << score << ")" << std::endl;
                    std::cout << "  Title: " << doc->GetTitle() << std::endl;
                    std::cout << "  Content: " << doc->GetContent() << std::endl;
                }
            }
        }
    }
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
}