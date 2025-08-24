/**
 * @file document_test.cpp
 * @brief Unit tests for the Document class.
 */

#include "document.h"
#include <gtest/gtest.h>

namespace search {

TEST(DocumentTest, ConstructorAndGetters) {
    // Test creating a document and getting its properties
    Document doc(1, "The quick brown fox", "Test Document");
    
    EXPECT_EQ(doc.GetId(), 1);
    EXPECT_EQ(doc.GetContent(), "The quick brown fox");
    EXPECT_EQ(doc.GetTitle(), "Test Document");
}

TEST(DocumentTest, TermExtraction) {
    // Test that terms are correctly extracted from content
    Document doc(1, "The quick brown fox jumps!", "Test Document");
    auto terms = doc.GetTerms();
    
    // Should have 4 terms: "the", "quick", "brown", "fox", "jumps"
    EXPECT_EQ(terms.size(), 5);
    
    // Check that punctuation is removed and terms are lowercase
    EXPECT_EQ(terms[0], "the");
    EXPECT_EQ(terms[1], "quick");
    EXPECT_EQ(terms[2], "brown");
    EXPECT_EQ(terms[3], "fox");
    EXPECT_EQ(terms[4], "jumps");
}

TEST(DocumentTest, EmptyContent) {
    // Test document with empty content
    Document doc(1, "", "Empty Document");
    auto terms = doc.GetTerms();
    
    EXPECT_EQ(terms.size(), 0);
}

} // namespace search