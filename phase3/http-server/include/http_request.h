#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>
#include <unordered_map>

namespace http_server {

/**
 * @brief Represents an HTTP request.
 * 
 * This class parses and stores the components of an HTTP request,
 * including the method, URI, version, headers, and message body.
 */
class HttpRequest {
public:
    /**
     * @brief Constructs an empty HttpRequest.
     */
    HttpRequest();

    /**
     * @brief Destructor.
     */
    ~HttpRequest();

    /**
     * @brief Parses an HTTP request from a string.
     * 
     * @param request_str The raw HTTP request string.
     * @return True if parsing was successful, false otherwise.
     */
    bool Parse(const std::string& request_str);

    /**
     * @brief Gets the HTTP method (e.g., "GET", "POST").
     * 
     * @return The HTTP method.
     */
    const std::string& GetMethod() const { return method_; }

    /**
     * @brief Gets the request URI.
     * 
     * @return The request URI.
     */
    const std::string& GetUri() const { return uri_; }

    /**
     * @brief Gets the HTTP version (e.g., "HTTP/1.1").
     * 
     * @return The HTTP version.
     */
    const std::string& GetVersion() const { return version_; }

    /**
     * @brief Gets a specific header value by name.
     * 
     * @param name The name of the header.
     * @return The value of the header, or an empty string if not found.
     */
    std::string GetHeader(const std::string& name) const;

    /**
     * @brief Gets the message body.
     * 
     * @return The message body.
     */
    const std::string& GetBody() const { return body_; }

private:
    std::string method_;     ///< HTTP method (e.g., "GET", "POST")
    std::string uri_;        ///< Request URI
    std::string version_;    ///< HTTP version (e.g., "HTTP/1.1")
    std::unordered_map<std::string, std::string> headers_; ///< HTTP headers
    std::string body_;       ///< Message body
};

} // namespace http_server

#endif // HTTP_REQUEST_H
