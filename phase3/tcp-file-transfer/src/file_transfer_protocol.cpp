#include "file_transfer_protocol.h"
#include <cstring>
#include <algorithm>

namespace tcp_file_transfer {

std::vector<char> SerializeMessage(const Message& msg) {
    // Calculate the total size of the message
    size_t filename_size = msg.filename.size() + 1; // +1 for null terminator
    size_t total_size = sizeof(uint32_t) * 2 + filename_size + msg.data.size();

    // Create the buffer
    std::vector<char> buffer(total_size);
    char* ptr = buffer.data();

    // Serialize type
    uint32_t type = msg.type;
    std::memcpy(ptr, &type, sizeof(type));
    ptr += sizeof(type);

    // Serialize length (filename + data size)
    uint32_t length = filename_size + msg.data.size();
    std::memcpy(ptr, &length, sizeof(length));
    ptr += sizeof(length);

    // Serialize filename
    std::memcpy(ptr, msg.filename.c_str(), msg.filename.size());
    ptr += msg.filename.size();
    *ptr = '\0'; // Null terminator
    ptr++;

    // Serialize data
    if (!msg.data.empty()) {
        std::memcpy(ptr, msg.data.data(), msg.data.size());
    }

    return buffer;
}

bool DeserializeMessage(const std::vector<char>& buffer, Message& msg) {
    if (buffer.size() < sizeof(uint32_t) * 2) {
        return false;
    }

    const char* ptr = buffer.data();

    // Deserialize type
    std::memcpy(&msg.type, ptr, sizeof(msg.type));
    ptr += sizeof(msg.type);

    // Deserialize length
    uint32_t length;
    std::memcpy(&length, ptr, sizeof(length));
    ptr += sizeof(length);

    // Check if we have enough data
    if (buffer.size() < sizeof(uint32_t) * 2 + length) {
        return false;
    }

    // Find the null terminator for the filename
    const char* filename_end = std::find(ptr, ptr + length, '\0');
    if (filename_end == ptr + length) {
        // No null terminator found
        return false;
    }

    // Deserialize filename
    msg.filename = std::string(ptr, filename_end);
    ptr = filename_end + 1;

    // Deserialize data
    size_t data_size = length - (ptr - (buffer.data() + sizeof(uint32_t) * 2));
    msg.data.assign(ptr, ptr + data_size);

    return true;
}

} // namespace tcp_file_transfer
