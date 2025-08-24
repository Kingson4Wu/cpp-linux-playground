#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "document.h"
#include "indexer.h"
#include "query_processor.h"
#include <vector>
#include <string>
#include <memory>

namespace search {

/**
 * @brief Search engine that indexes documents and processes queries
 * 
 * This class provides the main interface for the search engine, handling
 * document indexing and search query processing.
 */
class SearchEngine {
public:
    SearchEngine();
    ~SearchEngine() = default;

    // Delete copy constructor and assignment operator
    SearchEngine(const SearchEngine&) = delete;
    SearchEngine& operator=(const SearchEngine&) = delete;

    /**
     * @brief Add a document to the search engine
     * 
     * @param doc The document to add
     */
    void AddDocument(const Document& doc);

    /**
     * @brief Search for documents matching a query
     * 
     * @param query The search query
     * @param max_results Maximum number of results to return (default: 10)
     * @return std::vector<std::pair<int, double>> List of (document_id, score) pairs
     */
    std::vector<std::pair<int, double>> Search(const std::string& query, size_t max_results = 10) const;

    /**
     * @brief Get a document by ID
     * 
     * @param id The document ID
     * @return const Document* Pointer to the document, or nullptr if not found
     */
    const Document* GetDocument(int id) const;

private:
    Indexer indexer_;
    QueryProcessor query_processor_;
    mutable std::mutex documents_mutex_;
    std::unordered_map<int, std::unique_ptr<Document>> documents_;

    /**
     * @brief Calculate the TF-IDF score for a term in a document
     * 
     * @param term The term
     * @param doc_id The document ID
     * @return double The TF-IDF score
     */
    double CalculateTFIDF(const std::string& term, int doc_id) const;
};

} // namespace search

#endif // SEARCH_ENGINE_H