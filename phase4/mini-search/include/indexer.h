#ifndef INDEXER_H
#define INDEXER_H

#include "document.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include <set>

namespace search {

/**
 * @brief Inverted index for the search engine
 * 
 * This class maintains an inverted index mapping terms to the documents
 * that contain them. It is thread-safe for concurrent reads and writes.
 */
class Indexer {
public:
    Indexer() = default;
    ~Indexer() = default;

    // Delete copy constructor and assignment operator
    Indexer(const Indexer&) = delete;
    Indexer& operator=(const Indexer&) = delete;

    /**
     * @brief Add a document to the index
     * 
     * @param doc The document to add
     */
    void AddDocument(const Document& doc);

    /**
     * @brief Get the document IDs that contain a term
     * 
     * @param term The term to look up
     * @return std::vector<int> List of document IDs
     */
    std::vector<int> GetDocumentsForTerm(const std::string& term) const;

    /**
     * @brief Get the term frequency in a document
     * 
     * @param term The term to look up
     * @param doc_id The document ID
     * @return int The frequency of the term in the document
     */
    int GetTermFrequency(const std::string& term, int doc_id) const;

    /**
     * @brief Get the document frequency of a term (number of documents containing the term)
     * 
     * @param term The term to look up
     * @return int The document frequency
     */
    int GetDocumentFrequency(const std::string& term) const;

    /**
     * @brief Get the total number of documents in the index
     * 
     * @return size_t The number of documents
     */
    size_t GetDocumentCount() const;

private:
    // Inverted index: term -> set of document IDs
    mutable std::mutex index_mutex_;
    std::unordered_map<std::string, std::set<int>> inverted_index_;
    
    // Term frequency: (term, doc_id) -> frequency
    mutable std::mutex tf_mutex_;
    std::unordered_map<std::string, std::unordered_map<int, int>> term_frequency_;
    
    // Document frequency: term -> document count
    mutable std::mutex df_mutex_;
    std::unordered_map<std::string, int> document_frequency_;
    
    // Total number of documents
    mutable std::mutex doc_count_mutex_;
    size_t document_count_ = 0;
};

} // namespace search

#endif // INDEXER_H