#include "search_engine.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace search {

SearchEngine::SearchEngine() : query_processor_(indexer_) {}

void SearchEngine::AddDocument(const Document& doc) {
    // Add to indexer
    indexer_.AddDocument(doc);
    
    // Store document
    std::lock_guard<std::mutex> lock(documents_mutex_);
    documents_[doc.GetId()] = std::make_unique<Document>(doc);
}

std::vector<std::pair<int, double>> SearchEngine::Search(const std::string& query, size_t max_results) const {
    // Process the query
    std::vector<std::string> terms = query_processor_.ProcessQuery(query);
    
    if (terms.empty()) {
        return {};
    }
    
    // Map to store document scores
    std::unordered_map<int, double> doc_scores;
    
    // For each term in the query
    for (const std::string& term : terms) {
        // Get documents containing this term
        std::vector<int> doc_ids = indexer_.GetDocumentsForTerm(term);
        
        // Calculate TF-IDF score for each document
        for (int doc_id : doc_ids) {
            double tfidf = CalculateTFIDF(term, doc_id);
            doc_scores[doc_id] += tfidf;
        }
    }
    
    // Convert to vector and sort by score
    std::vector<std::pair<int, double>> results;
    for (const auto& pair : doc_scores) {
        results.push_back(pair);
    }
    
    // Sort by score (descending)
    std::sort(results.begin(), results.end(), 
              [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                  return a.second > b.second;
              });
    
    // Limit results
    if (results.size() > max_results) {
        results.resize(max_results);
    }
    
    return results;
}

const Document* SearchEngine::GetDocument(int id) const {
    std::lock_guard<std::mutex> lock(documents_mutex_);
    auto it = documents_.find(id);
    if (it != documents_.end()) {
        return it->second.get();
    }
    return nullptr;
}

double SearchEngine::CalculateTFIDF(const std::string& term, int doc_id) const {
    // Calculate term frequency (TF)
    int tf = indexer_.GetTermFrequency(term, doc_id);
    if (tf == 0) {
        return 0.0;
    }
    
    // Calculate document frequency (DF)
    int df = indexer_.GetDocumentFrequency(term);
    if (df == 0) {
        return 0.0;
    }
    
    // Calculate inverse document frequency (IDF)
    size_t total_docs = indexer_.GetDocumentCount();
    
    // Handle the case where a term appears in all documents
    double idf;
    if (df == total_docs) {
        // If the term appears in all documents, give it a small IDF value
        idf = 0.0001;
    } else {
        idf = std::log(static_cast<double>(total_docs) / df);
    }
    
    // Calculate TF-IDF score
    double tfidf = static_cast<double>(tf) * idf;
    
    return tfidf;
}

} // namespace search