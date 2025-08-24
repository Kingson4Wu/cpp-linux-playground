#ifndef HTTP_CONNECTION_HANDLER_H
#define HTTP_CONNECTION_HANDLER_H

#include "http_request.h"
#include "http_response.h"
#include <string>

namespace http_server {

/**
 * @brief Handles an HTTP connection.
 * 
 * This class is responsible for reading an HTTP request from a socket,
 * processing the request, and sending an HTTP response back to the client.
 */
class HttpConnectionHandler {
public:
    /**
     * @brief Constructs an HttpConnectionHandler.
     * 
     * @param client_socket The client socket file descriptor.
     * @param web_root The root directory for serving static files.
     * @param timeout_seconds The timeout for reading requests in seconds.
     */
    HttpConnectionHandler(int client_socket, const std::string& web_root, int timeout_seconds = 30);

    /**
     * @brief Destructor.
     */
    ~HttpConnectionHandler();

    /**
     * @brief Handles the HTTP connection.
     * 
     * This method reads the request, processes it, and sends the response.
     */
    void Handle();

private:
    int client_socket_; ///< Client socket file descriptor
    std::string web_root_; ///< Root directory for serving static files
    int timeout_seconds_; ///< Timeout for reading requests in seconds

    /**
     * @brief Reads the HTTP request from the client socket.
     * 
     * @param request The HttpRequest object to populate.
     * @return True if the request was read successfully, false otherwise.
     */
    bool ReadRequest(HttpRequest& request);

    /**
     * @brief Processes the HTTP request and generates a response.
     * 
     * @param request The incoming HTTP request.
     * @param response The HttpResponse object to populate.
     */
    void ProcessRequest(const HttpRequest& request, HttpResponse& response);

    /**
     * @brief Sends the HTTP response to the client socket.
     * 
     * @param response The HTTP response to send.
     * @return True if the response was sent successfully, false otherwise.
     */
    bool SendResponse(const HttpResponse& response);

    /**
     * @brief Serves a static file.
     * 
     * @param file_path The path to the file to serve.
     * @param response The HttpResponse object to populate.
     */
    void ServeStaticFile(const std::string& file_path, HttpResponse& response);

    /**
     * @brief Resolves a URI to a file path, preventing directory traversal.
     * 
     * @param uri The URI to resolve.
     * @return The resolved file path, or an empty string if invalid.
     */
    std::string ResolveUri(const std::string& uri) const;

    /**
     * @brief Gets the MIME type for a given file extension.
     * 
     * @param extension The file extension (e.g., ".html", ".css").
     * @return The MIME type (e.g., "text/html", "text/css").
     */
    std::string GetMimeType(const std::string& extension) const;
};

} // namespace http_server

#endif // HTTP_CONNECTION_HANDLER_H
