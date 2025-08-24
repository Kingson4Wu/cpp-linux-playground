#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <vector>

namespace search {

/**
 * @brief Represents a document in the search engine
 * 
 * This class holds the content and metadata of a document that can be indexed
 * and searched by the search engine.
 */
class Document {
public:
    /**
     * @brief Construct a new Document object
     * 
     * @param id Unique identifier for the document
     * @param content The content of the document
     * @param title The title of the document (optional)
     */
    Document(int id, const std::string& content, const std::string& title = "");

    /**
     * @brief Get the document ID
     * 
     * @return int The document ID
     */
    int GetId() const;

    /**
     * @brief Get the document content
     * 
     * @return const std::string& The document content
     */
    const std::string& GetContent() const;

    /**
     * @brief Get the document title
     * 
     * @return const std::string& The document title
     */
    const std::string& GetTitle() const;

    /**
     * @brief Get the terms (words) in the document
     * 
     * @return const std::vector<std::string>& List of terms
     */
    const std::vector<std::string>& GetTerms() const;

private:
    int id_;
    std::string content_;
    std::string title_;
    std::vector<std::string> terms_;

    /**
     * @brief Process the document content to extract terms
     */
    void ProcessContent();
};

} // namespace search

#endif // DOCUMENT_H