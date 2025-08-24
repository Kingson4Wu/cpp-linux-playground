#include "query_processor.h"
#include "indexer.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace search {

QueryProcessor::QueryProcessor(const Indexer& indexer) : indexer_(indexer) {}

std::vector<std::string> QueryProcessor::ProcessQuery(const std::string& query) const {
    std::vector<std::string> terms;
    
    // Simple tokenization - convert to lowercase and split on whitespace/punctuation
    std::string text = query;
    std::transform(text.begin(), text.end(), text.begin(), 
                   [](unsigned char c) { return std::tolower(c); });
    
    std::stringstream ss(text);
    std::string term;
    while (ss >> term) {
        // Remove punctuation
        term.erase(std::remove_if(term.begin(), term.end(), 
                                  [](unsigned char c) { return std::ispunct(c); }), 
                   term.end());
        
        // Add non-empty terms
        if (!term.empty()) {
            terms.push_back(term);
        }
    }
    
    return terms;
}

} // namespace search