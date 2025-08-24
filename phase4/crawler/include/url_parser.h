#ifndef URL_PARSER_H
#define URL_PARSER_H

#include <string>
#include <vector>

namespace crawler {

/**
 * @brief A simple URL parser
 * 
 * This class parses URLs and extracts components like protocol, host, port, path, etc.
 */
class UrlParser {
public:
    /**
     * @brief Construct a new UrlParser object
     * 
     * @param url The URL to parse
     */
    UrlParser(const std::string& url);

    /**
     * @brief Get the protocol (e.g., "http", "https")
     * 
     * @return const std::string& The protocol
     */
    const std::string& GetProtocol() const;

    /**
     * @brief Get the host (e.g., "www.example.com")
     * 
     * @return const std::string& The host
     */
    const std::string& GetHost() const;

    /**
     * @brief Get the port (e.g., 80, 443)
     * 
     * @return int The port
     */
    int GetPort() const;

    /**
     * @brief Get the path (e.g., "/path/to/resource")
     * 
     * @return const std::string& The path
     */
    const std::string& GetPath() const;

    /**
     * @brief Get the query string (e.g., "param1=value1&param2=value2")
     * 
     * @return const std::string& The query string
     */
    const std::string& GetQuery() const;

    /**
     * @brief Get the fragment (e.g., "section1")
     * 
     * @return const std::string& The fragment
     */
    const std::string& GetFragment() const;

    /**
     * @brief Check if the URL is valid
     * 
     * @return true If the URL is valid
     * @return false If the URL is invalid
     */
    bool IsValid() const;

private:
    std::string protocol_;
    std::string host_;
    int port_;
    std::string path_;
    std::string query_;
    std::string fragment_;
    bool valid_;

    /**
     * @brief Parse the URL
     * 
     * @param url The URL to parse
     */
    void Parse(const std::string& url);
};

} // namespace crawler

#endif // URL_PARSER_H