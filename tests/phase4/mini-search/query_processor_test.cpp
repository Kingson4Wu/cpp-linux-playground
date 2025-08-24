/**
 * @file query_processor_test.cpp
 * @brief Unit tests for the QueryProcessor class.
 */

#include "query_processor.h"
#include "indexer.h"
#include <gtest/gtest.h>

namespace search {

TEST(QueryProcessorTest, ProcessQuery) {
    // Test processing a query
    Indexer indexer;
    QueryProcessor processor(indexer);
    
    auto terms = processor.ProcessQuery("The quick brown fox!");
    
    // Should have 4 terms: "the", "quick", "brown", "fox"
    EXPECT_EQ(terms.size(), 4);
    
    // Check that punctuation is removed and terms are lowercase
    EXPECT_EQ(terms[0], "the");
    EXPECT_EQ(terms[1], "quick");
    EXPECT_EQ(terms[2], "brown");
    EXPECT_EQ(terms[3], "fox");
}

TEST(QueryProcessorTest, EmptyQuery) {
    // Test processing an empty query
    Indexer indexer;
    QueryProcessor processor(indexer);
    
    auto terms = processor.ProcessQuery("");
    EXPECT_EQ(terms.size(), 0);
}

TEST(QueryProcessorTest, QueryWithExtraSpaces) {
    // Test processing a query with extra spaces
    Indexer indexer;
    QueryProcessor processor(indexer);
    
    auto terms = processor.ProcessQuery("  The   quick  brown  ");
    EXPECT_EQ(terms.size(), 3);
    EXPECT_EQ(terms[0], "the");
    EXPECT_EQ(terms[1], "quick");
    EXPECT_EQ(terms[2], "brown");
}

} // namespace search