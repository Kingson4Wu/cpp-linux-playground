#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include "document.h"
#include <string>
#include <vector>
#include <memory>

namespace search {

// Forward declaration
class Indexer;

/**
 * @brief Processes search queries
 * 
 * This class handles parsing and processing of search queries,
 * converting them into a form that can be used to search the index.
 */
class QueryProcessor {
public:
    /**
     * @brief Construct a new Query Processor object
     * 
     * @param indexer Reference to the indexer to use for processing queries
     */
    explicit QueryProcessor(const Indexer& indexer);

    /**
     * @brief Process a search query
     * 
     * @param query The search query string
     * @return std::vector<std::string> The processed query terms
     */
    std::vector<std::string> ProcessQuery(const std::string& query) const;

private:
    const Indexer& indexer_;
};

} // namespace search

#endif // QUERY_PROCESSOR_H