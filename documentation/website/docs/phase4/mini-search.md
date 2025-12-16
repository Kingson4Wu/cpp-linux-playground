---
sidebar_position: 3
---

# mini-Search Implementation

Build a simplified search engine that can index documents and perform full-text search operations. This project combines data structures, algorithms, and system programming to create an efficient search solution.

## Project Structure

The mini-search is organized as follows:

```
phase4/mini-search/
├── CMakeLists.txt
├── include/
│   └── search_engine.h  # Search engine interface and classes
└── src/
    └── search_engine.cpp  # Implementation of search functionality
```

## Learning Objectives

During this project, you will:

- Implement inverted index data structures for efficient text search
- Design tokenization and text processing algorithms
- Build query parsing and execution systems
- Apply performance optimization for search operations
- Create document storage and retrieval mechanisms
- Implement ranking algorithms for search results
- Design memory-efficient storage for large document collections

## Core Concepts

### Inverted Index

The core data structure of search engines is an inverted index:

- **Forward Index**: Document → Terms (words in each document)
- **Inverted Index**: Term → Documents (documents containing each term)
- **Document Store**: Storage of actual document content with metadata

Example structure:
```
Term "hello": [doc1, doc5, doc10, ...]
Term "world": [doc2, doc5, doc7, ...]
```

### Text Processing

Implement proper text processing pipeline:

1. **Tokenization**: Split text into individual terms/words
2. **Normalization**: Convert to lowercase, remove punctuation
3. **Stemming** (optional): Reduce words to root forms
4. **Stop Word Removal** (optional): Remove common words like "the", "and"

## Implementation Architecture

### SearchEngine Interface

```cpp
class SearchEngine {
public:
    explicit SearchEngine(const std::string& data_dir);
    ~SearchEngine();
    
    // Indexing operations
    void add_document(int doc_id, const std::string& content, 
                     const std::string& metadata = "");
    void remove_document(int doc_id);
    void commit();  // Persist changes to disk
    
    // Search operations
    std::vector<SearchResult> search(const std::string& query, 
                                   size_t max_results = 10);
    
    // Utility operations
    size_t document_count() const;
    std::string get_document(int doc_id) const;
    
private:
    std::string data_dir_;
    
    // Core data structures
    std::unordered_map<std::string, std::vector<DocTermInfo>> inverted_index_;
    std::unordered_map<int, Document> documents_;
    std::unordered_map<std::string, int> term_frequencies_;
    
    // Synchronization for thread safety
    mutable std::shared_mutex index_mutex_;
    
    // Text processing utilities
    std::vector<std::string> tokenize(const std::string& text) const;
    std::string normalize(const std::string& token) const;
};
```

### Supporting Structures

```cpp
struct DocTermInfo {
    int doc_id;
    int term_frequency;  // How many times the term appears in document
    std::vector<int> positions;  // Positions where term appears
};

struct SearchResult {
    int doc_id;
    double score;  // Relevance score
    std::string snippet;  // Excerpt containing search terms
};

struct Document {
    std::string content;
    std::string metadata;
    std::chrono::system_clock::time_point timestamp;
};
```

## Implementation Requirements

### Core Features

1. **Document Indexing**: Add, update, and remove documents from the index
2. **Full-Text Search**: Find documents containing specified terms
3. **Phrase Search**: Support for multi-word searches
4. **Ranking**: Rank results by relevance (TF-IDF or similar)
5. **Persistence**: Save index to disk and load on startup
6. **Memory Efficiency**: Handle large document collections

### Text Processing Pipeline

Implement a robust text processing system:

- **Tokenization**: Split text using whitespace and punctuation as delimiters
- **Normalization**: Convert to lowercase, remove common punctuation
- **Filtering**: Optionally remove stop words (very common words)
- **Caching**: Cache processed terms to avoid re-processing

### Sample Implementation Approach

```cpp
class SearchEngine {
private:
    // Inverted index: term -> list of documents containing term
    std::unordered_map<std::string, std::vector<DocumentFrequency>> inverted_index_;
    
    // Forward index: document ID -> content and metadata
    std::unordered_map<int, Document> document_store_;
    
    // Statistics for ranking algorithms
    std::unordered_map<std::string, int> doc_frequency_;  // How many docs contain each term
    
    // Text processing utilities
    std::vector<std::string> tokenize_query(const std::string& query) const;
    double calculate_tf_idf_score(int doc_id, const std::vector<std::string>& terms) const;
    
public:
    void add_document(int doc_id, const std::string& content);
    std::vector<SearchResult> search(const std::string& query);
};
```

## Search Algorithms

### Basic Search Implementation

Start with a simple boolean search:

1. Parse the query into terms
2. Find all documents containing each term
3. Return documents that match all terms (AND operation)

### Advanced Ranking (Optional)

Implement TF-IDF (Term Frequency-Inverse Document Frequency) for better relevance:

- **TF**: How frequently a term appears in a document
- **IDF**: How rare the term is across all documents
- **Score**: TF × IDF for each term in document

## Performance Considerations

- **Index Size**: Keep index in memory for fast access
- **Compression**: Compress the index to reduce memory usage
- **Caching**: Cache frequent searches and results
- **Query Optimization**: Optimize query processing for common patterns
- **Memory Management**: Use appropriate data structures to minimize memory overhead

## Persistence Strategies

Consider implementing persistent storage options:

1. **Text-based format**: Simple JSON or custom text format (easy to debug)
2. **Binary format**: More efficient storage but harder to debug
3. **Database backend**: Use SQLite or similar for persistence (advanced)
4. **Memory mapping**: Map index files directly to memory for fast access

## Testing Strategy

Test your search engine with:

- Individual document indexing operations
- Various query types (single terms, phrase queries)
- Ranking accuracy of results
- Performance with large document collections
- Memory usage efficiency
- Persistence and recovery from restart

## Usage Example

```cpp
#include "search_engine.h"
#include <iostream>

int main() {
    try {
        SearchEngine engine("./search_data");
        
        // Add documents to index
        engine.add_document(1, "The quick brown fox jumps over the lazy dog");
        engine.add_document(2, "A journey of a thousand miles begins with a single step");
        engine.add_document(3, "To be or not to be, that is the question");
        
        // Commit changes to disk
        engine.commit();
        
        // Perform search
        auto results = engine.search("fox dog");
        std::cout << "Found " << results.size() << " results:" << std::endl;
        for (const auto& result : results) {
            std::cout << "Doc " << result.doc_id 
                      << " (score: " << result.score << ")" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Advanced Features (Optional)

Consider implementing additional capabilities:

1. **Phrase Search**: Support for "exact phrase" queries
2. **Boolean Queries**: AND, OR, NOT operations between terms
3. **Fuzzy Search**: Find similar terms using edit distance
4. **Faceted Search**: Filter results by document categories or attributes
5. **Real-time Indexing**: Update index without stopping the service
6. **Distributed Search**: Split index across multiple nodes

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# Create data directory
mkdir -p search_data

# The search engine can be used as a library in other applications
# or implemented with a simple command-line interface for testing
```

## Next Steps

After completing the search engine, move on to the [High-concurrency Crawler](./crawler.md) project to build a web crawler application.