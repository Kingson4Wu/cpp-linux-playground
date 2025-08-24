/**
 * @file debug_test.cpp
 * @brief Debug test for the search engine.
 */

#include "search_engine.h"
#include <iostream>
#include <memory>

int main() {
    search::SearchEngine engine;
    
    // Add some sample documents
    search::Document doc1(1, "The quick brown fox", "Doc 1");
    search::Document doc2(2, "A quick brown dog", "Doc 2");
    
    engine.AddDocument(doc1);
    engine.AddDocument(doc2);
    
    // Check document count
    std::cout << "Document 1 exists: " << (engine.GetDocument(1) != nullptr) << std::endl;
    std::cout << "Document 2 exists: " << (engine.GetDocument(2) != nullptr) << std::endl;
    
    // Try to search
    auto results = engine.Search("quick", 5);
    std::cout << "Search results for 'quick': " << results.size() << std::endl;
    
    for (const auto& result : results) {
        std::cout << "Document " << result.first << " Score: " << result.second << std::endl;
    }
    
    return 0;
}