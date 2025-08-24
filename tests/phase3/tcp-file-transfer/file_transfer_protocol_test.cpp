#include "file_transfer_protocol.h"
#include <gtest/gtest.h>
#include <cstring>

namespace tcp_file_transfer {

// Test serialization and deserialization of an upload request
TEST(FileTransferProtocolTest, SerializeDeserializeUploadRequest) {
    Message msg;
    msg.type = MSG_TYPE_UPLOAD_REQUEST;
    msg.filename = "test_file.txt";
    msg.data = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};

    // Serialize the message
    std::vector<char> buffer = SerializeMessage(msg);

    // Deserialize the message
    Message deserialized_msg;
    bool result = DeserializeMessage(buffer, deserialized_msg);

    // Check the result
    EXPECT_TRUE(result);
    EXPECT_EQ(deserialized_msg.type, msg.type);
    EXPECT_EQ(deserialized_msg.filename, msg.filename);
    EXPECT_EQ(deserialized_msg.data, msg.data);
}

// Test serialization and deserialization of a download request
TEST(FileTransferProtocolTest, SerializeDeserializeDownloadRequest) {
    Message msg;
    msg.type = MSG_TYPE_DOWNLOAD_REQUEST;
    msg.filename = "test_file.txt";
    msg.data = {}; // No data for download request

    // Serialize the message
    std::vector<char> buffer = SerializeMessage(msg);

    // Deserialize the message
    Message deserialized_msg;
    bool result = DeserializeMessage(buffer, deserialized_msg);

    // Check the result
    EXPECT_TRUE(result);
    EXPECT_EQ(deserialized_msg.type, msg.type);
    EXPECT_EQ(deserialized_msg.filename, msg.filename);
    EXPECT_EQ(deserialized_msg.data, msg.data);
}

// Test serialization and deserialization of an upload response
TEST(FileTransferProtocolTest, SerializeDeserializeUploadResponse) {
    Message msg;
    msg.type = MSG_TYPE_UPLOAD_RESPONSE;
    msg.filename = "test_file.txt";
    msg.data = {}; // No data for upload response

    // Serialize the message
    std::vector<char> buffer = SerializeMessage(msg);

    // Deserialize the message
    Message deserialized_msg;
    bool result = DeserializeMessage(buffer, deserialized_msg);

    // Check the result
    EXPECT_TRUE(result);
    EXPECT_EQ(deserialized_msg.type, msg.type);
    EXPECT_EQ(deserialized_msg.filename, msg.filename);
    EXPECT_EQ(deserialized_msg.data, msg.data);
}

// Test serialization and deserialization of a download response
TEST(FileTransferProtocolTest, SerializeDeserializeDownloadResponse) {
    Message msg;
    msg.type = MSG_TYPE_DOWNLOAD_RESPONSE;
    msg.filename = "test_file.txt";
    msg.data = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};

    // Serialize the message
    std::vector<char> buffer = SerializeMessage(msg);

    // Deserialize the message
    Message deserialized_msg;
    bool result = DeserializeMessage(buffer, deserialized_msg);

    // Check the result
    EXPECT_TRUE(result);
    EXPECT_EQ(deserialized_msg.type, msg.type);
    EXPECT_EQ(deserialized_msg.filename, msg.filename);
    EXPECT_EQ(deserialized_msg.data, msg.data);
}

// Test serialization and deserialization of an error message
TEST(FileTransferProtocolTest, SerializeDeserializeErrorMessage) {
    Message msg;
    msg.type = MSG_TYPE_ERROR;
    msg.filename = "test_file.txt";
    msg.data = {'E', 'r', 'r', 'o', 'r', ' ', 'm', 'e', 's', 's', 'a', 'g', 'e'};

    // Serialize the message
    std::vector<char> buffer = SerializeMessage(msg);

    // Deserialize the message
    Message deserialized_msg;
    bool result = DeserializeMessage(buffer, deserialized_msg);

    // Check the result
    EXPECT_TRUE(result);
    EXPECT_EQ(deserialized_msg.type, msg.type);
    EXPECT_EQ(deserialized_msg.filename, msg.filename);
    EXPECT_EQ(deserialized_msg.data, msg.data);
}

// Test deserialization of an invalid message (no null terminator)
TEST(FileTransferProtocolTest, DeserializeInvalidMessage) {
    // Create an invalid buffer with no null terminator
    std::vector<char> buffer = {
        0x01, 0x00, 0x00, 0x00, // Type: 1 (MSG_TYPE_UPLOAD_REQUEST)
        0x0D, 0x00, 0x00, 0x00, // Length: 13
        't', 'e', 's', 't', '_', 'f', 'i', 'l', 'e', '.', 't', 'x', 't', // Filename without null terminator
        'H', 'e', 'l', 'l', 'o'  // Data
    };

    // Deserialize the message
    Message msg;
    bool result = DeserializeMessage(buffer, msg);

    // Check the result
    EXPECT_FALSE(result);
}

} // namespace tcp_file_transfer
