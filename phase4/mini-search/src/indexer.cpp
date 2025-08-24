#include "indexer.h"
#include <algorithm>
#include <cctype>

namespace search {

void Indexer::AddDocument(const Document& doc) {
    int doc_id = doc.GetId();
    const std::vector<std::string>& terms = doc.GetTerms();

    // Lock for updating index
    std::lock_guard<std::mutex> lock(index_mutex_);
    
    // Count term frequencies in this document
    std::unordered_map<std::string, int> local_term_freq;
    for (const std::string& term : terms) {
        local_term_freq[term]++;
    }

    // Update the inverted index and term frequencies
    for (const auto& pair : local_term_freq) {
        const std::string& term = pair.first;
        int freq = pair.second;

        // Add document ID to inverted index
        inverted_index_[term].insert(doc_id);

        // Update term frequency
        {
            std::lock_guard<std::mutex> tf_lock(tf_mutex_);
            term_frequency_[term][doc_id] = freq;
        }

        // Update document frequency
        {
            std::lock_guard<std::mutex> df_lock(df_mutex_);
            document_frequency_[term]++;
        }
    }
    
    // Lock for updating document count
    {
        std::lock_guard<std::mutex> lock(doc_count_mutex_);
        document_count_++;
    }
}

std::vector<int> Indexer::GetDocumentsForTerm(const std::string& term) const {
    std::lock_guard<std::mutex> lock(index_mutex_);
    std::vector<int> result;
    
    auto it = inverted_index_.find(term);
    if (it != inverted_index_.end()) {
        // Convert set to vector
        for (int doc_id : it->second) {
            result.push_back(doc_id);
        }
    }
    
    return result;
}

int Indexer::GetTermFrequency(const std::string& term, int doc_id) const {
    std::lock_guard<std::mutex> lock(tf_mutex_);
    auto term_it = term_frequency_.find(term);
    if (term_it != term_frequency_.end()) {
        auto doc_it = term_it->second.find(doc_id);
        if (doc_it != term_it->second.end()) {
            return doc_it->second;
        }
    }
    return 0;
}

int Indexer::GetDocumentFrequency(const std::string& term) const {
    std::lock_guard<std::mutex> lock(df_mutex_);
    auto it = document_frequency_.find(term);
    if (it != document_frequency_.end()) {
        return it->second;
    }
    return 0;
}

size_t Indexer::GetDocumentCount() const {
    std::lock_guard<std::mutex> lock(doc_count_mutex_);
    return document_count_;
}

} // namespace search