#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <unordered_map>

namespace http_server {

/**
 * @brief Represents an HTTP response.
 * 
 * This class builds and stores the components of an HTTP response,
 * including the status code, headers, and message body.
 */
class HttpResponse {
public:
    /**
     * @brief Constructs an HttpResponse with a given status code.
     * 
     * @param status_code The HTTP status code (e.g., 200, 404).
     */
    explicit HttpResponse(int status_code = 200);

    /**
     * @brief Destructor.
     */
    ~HttpResponse();

    /**
     * @brief Sets the status code.
     * 
     * @param status_code The HTTP status code.
     */
    void SetStatusCode(int status_code) { status_code_ = status_code; }

    /**
     * @brief Gets the status code.
     * 
     * @return The HTTP status code.
     */
    int GetStatusCode() const { return status_code_; }

    /**
     * @brief Sets a header field.
     * 
     * @param name The name of the header.
     * @param value The value of the header.
     */
    void SetHeader(const std::string& name, const std::string& value);

    /**
     * @brief Gets a specific header value by name.
     * 
     * @param name The name of the header.
     * @return The value of the header, or an empty string if not found.
     */
    std::string GetHeader(const std::string& name) const;

    /**
     * @brief Sets the message body.
     * 
     * @param body The message body.
     */
    void SetBody(const std::string& body) { body_ = body; }

    /**
     * @brief Gets the message body.
     * 
     * @return The message body.
     */
    const std::string& GetBody() const { return body_; }

    /**
     * @brief Converts the response to a string suitable for sending over a socket.
     * 
     * @return The serialized HTTP response.
     */
    std::string ToString() const;

private:
    int status_code_; ///< HTTP status code
    std::unordered_map<std::string, std::string> headers_; ///< HTTP headers
    std::string body_; ///< Message body

    /**
     * @brief Gets the reason phrase for a given status code.
     * 
     * @param status_code The HTTP status code.
     * @return The reason phrase.
     */
    std::string GetReasonPhrase(int status_code) const;
};

} // namespace http_server

#endif // HTTP_RESPONSE_H
