#ifndef TCP_FILE_SERVER_H
#define TCP_FILE_SERVER_H

#include "thread_pool.h"
#include <string>
#include <atomic>
#include <sys/socket.h>

namespace tcp_file_transfer {

class TcpFileServer {
public:
    TcpFileServer(int port, const std::string& file_storage_path);
    ~TcpFileServer();

    // Delete copy constructor and assignment operator
    TcpFileServer(const TcpFileServer&) = delete;
    TcpFileServer& operator=(const TcpFileServer&) = delete;

    // Start the server
    bool Start();

    // Stop the server
    void Stop();

private:
    int port_;
    std::string file_storage_path_;
    std::atomic<bool> stop_;
    int server_socket_;
    ThreadPool thread_pool_;

    // Handle a client connection
    void HandleClient(int client_socket);

    // Process a message from a client
    void ProcessMessage(int client_socket);

    // Handle upload request
    void HandleUploadRequest(int client_socket, const std::string& filename, const std::vector<char>& data);

    // Handle download request
    void HandleDownloadRequest(int client_socket, const std::string& filename);

    // Send a response to the client
    bool SendResponse(int client_socket, uint32_t type, const std::string& filename, const std::vector<char>& data);

    // Resolve a filename to a safe file path
    std::string ResolveFilePath(const std::string& filename) const;
};

} // namespace tcp_file_transfer

#endif // TCP_FILE_SERVER_H
