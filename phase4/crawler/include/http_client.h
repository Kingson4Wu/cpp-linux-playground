#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "url_parser.h"
#include <string>
#include <map>
#include <memory>

namespace crawler {

/**
 * @brief A simple HTTP client
 * 
 * This class handles HTTP requests and responses.
 */
class HttpClient {
public:
    /**
     * @brief HTTP response structure
     */
    struct HttpResponse {
        int status_code;
        std::map<std::string, std::string> headers;
        std::string body;
    };

    /**
     * @brief Construct a new Http Client object
     * 
     * @param timeout_seconds Timeout for network operations in seconds
     */
    HttpClient(int timeout_seconds = 30);

    /**
     * @brief Destroy the Http Client object
     */
    ~HttpClient();

    /**
     * @brief Send an HTTP GET request
     * 
     * @param url The URL to request
     * @return std::unique_ptr<HttpResponse> The response, or nullptr on error
     */
    std::unique_ptr<HttpResponse> Get(const std::string& url);

private:
    int timeout_seconds_;

    /**
     * @brief Parse an HTTP response
     * 
     * @param response_data The raw response data
     * @return std::unique_ptr<HttpResponse> The parsed response
     */
    std::unique_ptr<HttpResponse> ParseResponse(const std::string& response_data);
};

} // namespace crawler

#endif // HTTP_CLIENT_H