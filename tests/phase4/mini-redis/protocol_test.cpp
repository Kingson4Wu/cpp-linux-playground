/**
 * @file protocol_test.cpp
 * @brief Unit tests for the Redis protocol parser.
 */

#include "protocol.h"
#include <gtest/gtest.h>

namespace redis {

class ProtocolTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up code here if needed
    }

    void TearDown() override {
        // Clean up code here if needed
    }

    Protocol protocol;
};

TEST_F(ProtocolTest, SimpleString) {
    // Test parsing and serializing a simple string
    std::string data = "+OK\r
";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, RESPType::SIMPLE_STRING);

    auto simple_string = static_cast<SimpleString*>(value.get());
    EXPECT_EQ(simple_string->value, "OK");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, Error) {
    // Test parsing and serializing an error
    std::string data = "-Error message\r
";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, RESPType::ERROR);

    auto error = static_cast<Error*>(value.get());
    EXPECT_EQ(error->message, "Error message");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, Integer) {
    // Test parsing and serializing an integer
    std::string data = ":1000\r
";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, RESPType::INTEGER);

    auto integer = static_cast<Integer*>(value.get());
    EXPECT_EQ(integer->value, 1000);

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, BulkString) {
    // Test parsing and serializing a bulk string
    std::string data = "$5\r
hello\r
";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, RESPType::BULK_STRING);

    auto bulk_string = static_cast<BulkString*>(value.get());
    EXPECT_TRUE(bulk_string->value.has_value());
    EXPECT_EQ(bulk_string->value.value(), "hello");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, NullBulkString) {
    // Test parsing and serializing a null bulk string
    std::string data = "$-1\r
";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, RESPType::BULK_STRING);

    auto bulk_string = static_cast<BulkString*>(value.get());
    EXPECT_FALSE(bulk_string->value.has_value());

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, Array) {
    // Test parsing and serializing an array
    std::string data = "*2\r
$5\r
hello\r
$5\r
world\r
";
    auto value = protocol.Parse(data);
    EXPECT_NE(value, nullptr);
    EXPECT_EQ(value->type, RESPType::ARRAY);

    auto array = static_cast<Array*>(value.get());
    EXPECT_EQ(array->elements.size(), 2);

    // Check first element
    EXPECT_EQ(array->elements[0]->type, RESPType::BULK_STRING);
    auto first_element = static_cast<BulkString*>(array->elements[0].get());
    EXPECT_TRUE(first_element->value.has_value());
    EXPECT_EQ(first_element->value.value(), "hello");

    // Check second element
    EXPECT_EQ(array->elements[1]->type, RESPType::BULK_STRING);
    auto second_element = static_cast<BulkString*>(array->elements[1].get());
    EXPECT_TRUE(second_element->value.has_value());
    EXPECT_EQ(second_element->value.value(), "world");

    // Test serializing
    std::string serialized = protocol.Serialize(*value);
    EXPECT_EQ(serialized, data);
}

TEST_F(ProtocolTest, CreateAndSerialize) {
    // Test creating and serializing different types of values

    // Simple string
    auto simple_string = protocol.CreateSimpleString("OK");
    std::string serialized = protocol.Serialize(*simple_string);
    EXPECT_EQ(serialized, "+OK\r
");

    // Error
    auto error = protocol.CreateError("Error message");
    serialized = protocol.Serialize(*error);
    EXPECT_EQ(serialized, "-Error message\r
");

    // Integer
    auto integer = protocol.CreateInteger(1000);
    serialized = protocol.Serialize(*integer);
    EXPECT_EQ(serialized, ":1000\r
");

    // Bulk string
    auto bulk_string = protocol.CreateBulkString("hello");
    serialized = protocol.Serialize(*bulk_string);
    EXPECT_EQ(serialized, "$5\r
hello\r
");

    // Null bulk string
    auto null_bulk_string = protocol.CreateNullBulkString();
    serialized = protocol.Serialize(*null_bulk_string);
    EXPECT_EQ(serialized, "$-1\r
");

    // Array
    auto array = protocol.CreateArray();
    array->AddElement(std::move(protocol.CreateBulkString("hello")));
    array->AddElement(std::move(protocol.CreateBulkString("world")));
    serialized = protocol.Serialize(*array);
    EXPECT_EQ(serialized, "*2\r
$5\r
hello\r
$5\r
world\r
");
}

} // namespace redis
