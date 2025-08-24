/**
 * @file indexer_test.cpp
 * @brief Unit tests for the Indexer class.
 */

#include "indexer.h"
#include "document.h"
#include <gtest/gtest.h>

namespace search {

TEST(IndexerTest, AddDocument) {
    // Test adding a document to the indexer
    Indexer indexer;
    Document doc(1, "The quick brown fox jumps over the lazy dog", "Test Document");
    
    indexer.AddDocument(doc);
    
    EXPECT_EQ(indexer.GetDocumentCount(), 1);
}

TEST(IndexerTest, GetDocumentsForTerm) {
    // Test retrieving documents for a term
    Indexer indexer;
    Document doc(1, "The quick brown fox", "Test Document");
    
    indexer.AddDocument(doc);
    
    auto doc_ids = indexer.GetDocumentsForTerm("quick");
    EXPECT_EQ(doc_ids.size(), 1);
    EXPECT_EQ(doc_ids[0], 1);
    
    // Test for non-existent term
    auto empty_ids = indexer.GetDocumentsForTerm("nonexistent");
    EXPECT_EQ(empty_ids.size(), 0);
}

TEST(IndexerTest, GetTermFrequency) {
    // Test getting term frequency
    Indexer indexer;
    Document doc(1, "The quick brown fox jumps quick", "Test Document");
    
    indexer.AddDocument(doc);
    
    EXPECT_EQ(indexer.GetTermFrequency("quick", 1), 2);
    EXPECT_EQ(indexer.GetTermFrequency("brown", 1), 1);
    EXPECT_EQ(indexer.GetTermFrequency("nonexistent", 1), 0);
}

TEST(IndexerTest, GetDocumentFrequency) {
    // Test getting document frequency
    Indexer indexer;
    Document doc1(1, "The quick brown fox", "Test Document 1");
    Document doc2(2, "The quick dog", "Test Document 2");
    
    indexer.AddDocument(doc1);
    indexer.AddDocument(doc2);
    
    EXPECT_EQ(indexer.GetDocumentFrequency("quick"), 2);
    EXPECT_EQ(indexer.GetDocumentFrequency("brown"), 1);
    EXPECT_EQ(indexer.GetDocumentFrequency("nonexistent"), 0);
}

TEST(IndexerTest, MultipleDocuments) {
    // Test with multiple documents
    Indexer indexer;
    Document doc1(1, "The quick brown fox", "Test Document 1");
    Document doc2(2, "The lazy dog", "Test Document 2");
    Document doc3(3, "The quick lazy cat", "Test Document 3");
    
    indexer.AddDocument(doc1);
    indexer.AddDocument(doc2);
    indexer.AddDocument(doc3);
    
    EXPECT_EQ(indexer.GetDocumentCount(), 3);
    
    // Test documents for "quick"
    auto quick_docs = indexer.GetDocumentsForTerm("quick");
    EXPECT_EQ(quick_docs.size(), 2);
    EXPECT_TRUE(std::find(quick_docs.begin(), quick_docs.end(), 1) != quick_docs.end());
    EXPECT_TRUE(std::find(quick_docs.begin(), quick_docs.end(), 3) != quick_docs.end());
    
    // Test documents for "the"
    auto the_docs = indexer.GetDocumentsForTerm("the");
    EXPECT_EQ(the_docs.size(), 3);
}

} // namespace search