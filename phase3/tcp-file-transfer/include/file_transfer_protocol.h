#ifndef FILE_TRANSFER_PROTOCOL_H
#define FILE_TRANSFER_PROTOCOL_H

#include <string>
#include <vector>
#include <cstdint>

namespace tcp_file_transfer {

// Message types
constexpr uint32_t MSG_TYPE_UPLOAD_REQUEST = 1;
constexpr uint32_t MSG_TYPE_DOWNLOAD_REQUEST = 2;
constexpr uint32_t MSG_TYPE_UPLOAD_RESPONSE = 3;
constexpr uint32_t MSG_TYPE_DOWNLOAD_RESPONSE = 4;
constexpr uint32_t MSG_TYPE_ERROR = 5;

// Maximum file size (100MB)
constexpr size_t MAX_FILE_SIZE = 100 * 1024 * 1024;

// Message structure
struct Message {
    uint32_t type;
    std::string filename;
    std::vector<char> data;
};

// Serialize a message to a byte vector
std::vector<char> SerializeMessage(const Message& msg);

// Deserialize a byte vector to a message
bool DeserializeMessage(const std::vector<char>& buffer, Message& msg);

} // namespace tcp_file_transfer

#endif // FILE_TRANSFER_PROTOCOL_H
