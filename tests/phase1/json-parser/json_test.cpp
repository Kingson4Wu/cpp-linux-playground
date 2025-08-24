/**
 * @file json_test.cpp
 * @brief Unit tests for the JSON parser library using Google Test.
 */

#include "json.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

// Test fixture for Json tests
class JsonTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a unique temporary directory for testing
        test_dir = std::filesystem::temp_directory_path() / "json_test_dir";
        std::filesystem::create_directory(test_dir);
    }

    void TearDown() override {
        // Clean up the temporary directory
        std::filesystem::remove_all(test_dir);
    }

    std::filesystem::path test_dir;
};

// Test to check if JsonValue can parse null
TEST_F(JsonTest, ParsesNull) {
    std::string json_str = "null";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_null());
}

// Test to check if JsonValue can parse true
TEST_F(JsonTest, ParsesTrue) {
    std::string json_str = "true";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_bool());
    EXPECT_TRUE(value.as_bool());
}

// Test to check if JsonValue can parse false
TEST_F(JsonTest, ParsesFalse) {
    std::string json_str = "false";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_bool());
    EXPECT_FALSE(value.as_bool());
}

// Test to check if JsonValue can parse a number
TEST_F(JsonTest, ParsesNumber) {
    std::string json_str = "42.5";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_number());
    EXPECT_DOUBLE_EQ(value.as_number(), 42.5);
}

// Test to check if JsonValue can parse a string
TEST_F(JsonTest, ParsesString) {
    std::string json_str = "\"Hello, World!\"";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_string());
    EXPECT_EQ(value.as_string(), "Hello, World!");
}

// Test to check if JsonValue can parse an empty array
TEST_F(JsonTest, ParsesEmptyArray) {
    std::string json_str = "[]";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_array());
    EXPECT_EQ(value.as_array().size(), 0);
}

// Test to check if JsonValue can parse an array
TEST_F(JsonTest, ParsesArray) {
    std::string json_str = "[1, 2, 3]";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_array());
    EXPECT_EQ(value.as_array().size(), 3);
    EXPECT_DOUBLE_EQ(value.as_array()[0].as_number(), 1.0);
    EXPECT_DOUBLE_EQ(value.as_array()[1].as_number(), 2.0);
    EXPECT_DOUBLE_EQ(value.as_array()[2].as_number(), 3.0);
}

// Test to check if JsonValue can parse an empty object
TEST_F(JsonTest, ParsesEmptyObject) {
    std::string json_str = "{}";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_object());
    EXPECT_EQ(value.as_object().size(), 0);
}

// Test to check if JsonValue can parse a simple object
TEST_F(JsonTest, ParsesSimpleObject) {
    std::string json_str = "{\"key\": \"value\"}";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_object());
    EXPECT_EQ(value.as_object().size(), 1);
    EXPECT_EQ(value.as_object().at("key").as_string(), "value");
}

// Test to check if JsonValue can parse a complex object
TEST_F(JsonTest, ParsesComplexObject) {
    std::string json_str = "{\"name\": \"John\", \"age\": 30, \"isStudent\": false, \"courses\": [\"Math\", \"Physics\"]}";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_object());
    EXPECT_EQ(value.as_object().size(), 4);
    
    EXPECT_EQ(value.as_object().at("name").as_string(), "John");
    EXPECT_DOUBLE_EQ(value.as_object().at("age").as_number(), 30.0);
    EXPECT_FALSE(value.as_object().at("isStudent").as_bool());
    
    const JsonArray& courses = value.as_object().at("courses").as_array();
    EXPECT_EQ(courses.size(), 2);
    EXPECT_EQ(courses[0].as_string(), "Math");
    EXPECT_EQ(courses[1].as_string(), "Physics");
}

// Test to check if JsonValue can serialize a value
TEST_F(JsonTest, SerializesValue) {
    JsonValue value("Hello, World!");
    std::string serialized = value.to_string();
    EXPECT_EQ(serialized, "\"Hello, World!\"");
}

// Test to check if JsonValue can handle escaped characters
TEST_F(JsonTest, HandlesEscapedCharacters) {
    std::string json_str = "\"Hello\\nWorld\"";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_string());
    EXPECT_EQ(value.as_string(), "Hello\nWorld");
}

// Test to check if JsonValue can parse from a file
TEST_F(JsonTest, ParsesFromFile) {
    // Create a test JSON file
    auto test_file = test_dir / "test.json";
    std::ofstream file(test_file);
    file << "{\"key\": \"value\"}";
    file.close();

    // Parse the file
    JsonValue value = parse_json_file(test_file);
    EXPECT_TRUE(value.is_object());
    EXPECT_EQ(value.as_object().size(), 1);
    EXPECT_EQ(value.as_object().at("key").as_string(), "value");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}