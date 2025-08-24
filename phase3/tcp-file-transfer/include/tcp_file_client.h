#ifndef TCP_FILE_CLIENT_H
#define TCP_FILE_CLIENT_H

#include <string>
#include <vector>
#include <sys/socket.h>
#include <cstdint>

namespace tcp_file_transfer {

class TcpFileClient {
public:
    TcpFileClient();
    ~TcpFileClient();

    // Delete copy constructor and assignment operator
    TcpFileClient(const TcpFileClient&) = delete;
    TcpFileClient& operator=(const TcpFileClient&) = delete;

    // Connect to the server
    bool Connect(const std::string& server_ip, int port);

    // Disconnect from the server
    void Disconnect();

    // Upload a file to the server
    bool UploadFile(const std::string& local_file_path, const std::string& remote_filename);

    // Download a file from the server
    bool DownloadFile(const std::string& remote_filename, const std::string& local_file_path);

private:
    int client_socket_;

    // Send a message to the server
    bool SendMessage(uint32_t type, const std::string& filename, const std::vector<char>& data);

    // Receive a message from the server
    bool ReceiveMessage(uint32_t& type, std::string& filename, std::vector<char>& data);
};

} // namespace tcp_file_transfer

#endif // TCP_FILE_CLIENT_H
