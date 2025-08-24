/**
 * @file protocol_test.cpp
 * @brief Unit tests for the Redis protocol library using Google Test.
 *
 * These tests cover the core functionality of the Redis protocol library,
 * including parsing and serialization of different RESP data types.
 */

#include "protocol.h"
#include <gtest/gtest.h>
#include <string>
#include <memory>

// Test fixture for Protocol tests
class ProtocolTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code, if needed
    }

    void TearDown() override {
        // Cleanup code, if needed
    }

    redis::Protocol protocol;
};

TEST_F(ProtocolTest, SimpleString) {
    // Test parsing and serializing a simple string
    std::string data = "+OK\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::SIMPLE_STRING);

    auto simple_string = static_cast<redis::SimpleString*>(value.get());
    EXPECT_EQ(simple_string->value, "OK");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, Error) {
    // Test parsing and serializing an error
    std::string data = "-Error message\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::ERROR);

    auto error = static_cast<redis::Error*>(value.get());
    EXPECT_EQ(error->message, "Error message");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, Integer) {
    // Test parsing and serializing an integer
    std::string data = ":1000\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::INTEGER);

    auto integer = static_cast<redis::Integer*>(value.get());
    EXPECT_EQ(integer->value, 1000);

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, BulkString) {
    // Test parsing and serializing a bulk string
    std::string data = "$5\r\nhello\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::BULK_STRING);

    auto bulk_string = static_cast<redis::BulkString*>(value.get());
    EXPECT_TRUE(bulk_string->value.has_value());
    EXPECT_EQ(bulk_string->value.value(), "hello");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, NullBulkString) {
    // Test parsing and serializing a null bulk string
    std::string data = "$-1\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::BULK_STRING);

    auto bulk_string = static_cast<redis::BulkString*>(value.get());
    EXPECT_FALSE(bulk_string->value.has_value());

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, Array) {
    // Test parsing and serializing an array
    std::string data = "*2\r\n$5\r\nhello\r\n$5\r\nworld\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::ARRAY);

    auto array = static_cast<redis::Array*>(value.get());
    EXPECT_EQ(array->elements.size(), 2);

    // Check first element
    auto first_element = static_cast<redis::BulkString*>(array->elements[0].get());
    EXPECT_TRUE(first_element->value.has_value());
    EXPECT_EQ(first_element->value.value(), "hello");

    // Check second element
    auto second_element = static_cast<redis::BulkString*>(array->elements[1].get());
    EXPECT_TRUE(second_element->value.has_value());
    EXPECT_EQ(second_element->value.value(), "world");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, NullArray) {
    // Test parsing and serializing a null array
    std::string data = "*-1\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::ARRAY);

    auto array = static_cast<redis::Array*>(value.get());
    EXPECT_EQ(array->elements.size(), 0);

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, EmptyArray) {
    // Test parsing and serializing an empty array
    std::string data = "*0\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::ARRAY);

    auto array = static_cast<redis::Array*>(value.get());
    EXPECT_EQ(array->elements.size(), 0);

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, MixedArray) {
    // Test parsing and serializing an array with mixed types
    std::string data = "*3\r\n+OK\r\n:1000\r\n$5\r\nhello\r\n";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, redis::RESPType::ARRAY);

    auto array = static_cast<redis::Array*>(value.get());
    EXPECT_EQ(array->elements.size(), 3);

    // Check first element (simple string)
    auto first_element = static_cast<redis::SimpleString*>(array->elements[0].get());
    EXPECT_EQ(first_element->value, "OK");

    // Check second element (integer)
    auto second_element = static_cast<redis::Integer*>(array->elements[1].get());
    EXPECT_EQ(second_element->value, 1000);

    // Check third element (bulk string)
    auto third_element = static_cast<redis::BulkString*>(array->elements[2].get());
    EXPECT_TRUE(third_element->value.has_value());
    EXPECT_EQ(third_element->value.value(), "hello");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}