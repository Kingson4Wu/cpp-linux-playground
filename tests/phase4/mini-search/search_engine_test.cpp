/**
 * @file search_engine_test.cpp
 * @brief Unit tests for the SearchEngine class.
 */

#include "search_engine.h"
#include <gtest/gtest.h>
#include <cmath>

namespace search {

TEST(SearchEngineTest, AddDocument) {
    // Test adding a document to the search engine
    SearchEngine engine;
    Document doc(1, "The quick brown fox", "Test Document");
    
    engine.AddDocument(doc);
    
    // Try to retrieve the document
    const Document* retrieved_doc = engine.GetDocument(1);
    ASSERT_NE(retrieved_doc, nullptr);
    EXPECT_EQ(retrieved_doc->GetId(), 1);
    EXPECT_EQ(retrieved_doc->GetContent(), "The quick brown fox");
}

TEST(SearchEngineTest, Search) {
    // Test searching for documents
    SearchEngine engine;
    Document doc1(1, "The quick brown fox jumps over the lazy dog", "Doc 1");
    Document doc2(2, "A quick brown dog jumps over a lazy fox", "Doc 2");
    Document doc3(3, "The lazy dog and fox are quick and brown", "Doc 3");
    
    engine.AddDocument(doc1);
    engine.AddDocument(doc2);
    engine.AddDocument(doc3);
    
    // Search for "quick"
    auto results = engine.Search("quick", 5);
    
    // Should have 3 results
    EXPECT_EQ(results.size(), 3);
    
    // All documents contain "quick", so all should have a score > 0
    for (const auto& result : results) {
        EXPECT_GT(result.second, 0.0);
    }
}

TEST(SearchEngineTest, SearchNoResults) {
    // Test searching for a term that doesn't exist
    SearchEngine engine;
    Document doc(1, "The quick brown fox", "Test Document");
    
    engine.AddDocument(doc);
    
    auto results = engine.Search("nonexistent", 5);
    EXPECT_EQ(results.size(), 0);
}

TEST(SearchEngineTest, SearchLimitResults) {
    // Test limiting the number of search results
    SearchEngine engine;
    
    // Add 5 documents with the same term
    for (int i = 1; i <= 5; i++) {
        std::string content = "Document with term " + std::to_string(i);
        Document doc(i, content, "Doc " + std::to_string(i));
        engine.AddDocument(doc);
    }
    
    // Search with a limit of 3 results
    auto results = engine.Search("term", 3);
    EXPECT_EQ(results.size(), 3);
}

TEST(SearchEngineTest, CalculateTFIDF) {
    // Test TF-IDF calculation (indirectly through search)
    SearchEngine engine;
    Document doc1(1, "The quick brown fox", "Doc 1");
    Document doc2(2, "The lazy dog", "Doc 2");
    
    engine.AddDocument(doc1);
    engine.AddDocument(doc2);
    
    // Search for "quick" which only appears in doc1
    auto results = engine.Search("quick", 5);
    
    // Should have 1 result
    EXPECT_EQ(results.size(), 1);
    
    // The result should be doc1
    EXPECT_EQ(results[0].first, 1);
    
    // The score should be greater than 0
    EXPECT_GT(results[0].second, 0.0);
}

} // namespace search