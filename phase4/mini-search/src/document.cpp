#include "document.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace search {

Document::Document(int id, const std::string& content, const std::string& title)
    : id_(id), content_(content), title_(title) {
    ProcessContent();
}

int Document::GetId() const {
    return id_;
}

const std::string& Document::GetContent() const {
    return content_;
}

const std::string& Document::GetTitle() const {
    return title_;
}

const std::vector<std::string>& Document::GetTerms() const {
    return terms_;
}

void Document::ProcessContent() {
    // Simple tokenization - convert to lowercase and split on whitespace/punctuation
    std::string text = content_;
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
            terms_.push_back(term);
        }
    }
}

} // namespace search