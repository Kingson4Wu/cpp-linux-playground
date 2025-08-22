/*
 * json_test.cpp - Unit tests for the JSON parser implementation
 *
 * How to run these tests:
 *
 * 1. Build the project:
 *    mkdir -p build && cd build && cmake .. && make
 *
 * 2. Run all tests:
 *    ctest
 *
 * 3. Run the test executable directly:
 *    ./build/phase1/json-parser/json_test
 *
 * 4. Run a specific test case:
 *    ./build/phase1/json-parser/json_test --gtest_filter=JsonTest.ParsesSimpleObject
 */

#include "gtest/gtest.h"
#include "json.h"
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

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

TEST_F(JsonTest, ParsesNull) {
    std::string json_str = "null";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_null());
}

TEST_F(JsonTest, ParsesTrue) {
    std::string json_str = "true";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_bool());
    EXPECT_TRUE(value.as_bool());
}

TEST_F(JsonTest, ParsesFalse) {
    std::string json_str = "false";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_bool());
    EXPECT_FALSE(value.as_bool());
}

TEST_F(JsonTest, ParsesNumber) {
    std::string json_str = "123.456";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_number());
    EXPECT_DOUBLE_EQ(value.as_number(), 123.456);
}

TEST_F(JsonTest, ParsesString) {
    std::string json_str = "\"Hello, World!\"";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_string());
    EXPECT_EQ(value.as_string(), "Hello, World!");
}

TEST_F(JsonTest, ParsesEmptyArray) {
    std::string json_str = "[]";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_array());
    EXPECT_EQ(value.as_array().size(), 0);
}

TEST_F(JsonTest, ParsesArray) {
    std::string json_str = "[1, 2, 3]";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_array());
    const JsonArray& arr = value.as_array();
    EXPECT_EQ(arr.size(), 3);
    EXPECT_DOUBLE_EQ(arr[0].as_number(), 1);
    EXPECT_DOUBLE_EQ(arr[1].as_number(), 2);
    EXPECT_DOUBLE_EQ(arr[2].as_number(), 3);
}

TEST_F(JsonTest, ParsesEmptyObject) {
    std::string json_str = "{}";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_object());
    EXPECT_EQ(value.as_object().size(), 0);
}

TEST_F(JsonTest, ParsesSimpleObject) {
    std::string json_str = "{\"key\": \"value\"}";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_object());
    const JsonObject& obj = value.as_object();
    EXPECT_EQ(obj.size(), 1);
    EXPECT_EQ(obj.at("key").as_string(), "value");
}

TEST_F(JsonTest, ParsesComplexObject) {
    std::string json_str = R"({
        "name": "John Doe",
        "age": 30,
        "isStudent": false,
        "courses": ["Math", "Physics"],
        "address": {
            "street": "123 Main St",
            "city": "Anytown"
        }
    })";

    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_object());
    const JsonObject& obj = value.as_object();

    EXPECT_EQ(obj.at("name").as_string(), "John Doe");
    EXPECT_DOUBLE_EQ(obj.at("age").as_number(), 30);
    EXPECT_FALSE(obj.at("isStudent").as_bool());

    const JsonArray& courses = obj.at("courses").as_array();
    EXPECT_EQ(courses.size(), 2);
    EXPECT_EQ(courses[0].as_string(), "Math");
    EXPECT_EQ(courses[1].as_string(), "Physics");

    const JsonObject& address = obj.at("address").as_object();
    EXPECT_EQ(address.at("street").as_string(), "123 Main St");
    EXPECT_EQ(address.at("city").as_string(), "Anytown");
}

TEST_F(JsonTest, SerializesValue) {
    JsonObject obj;
    obj["name"] = JsonValue("John Doe");
    obj["age"] = JsonValue(30);
    obj["isStudent"] = JsonValue(false);

    JsonArray courses;
    courses.push_back(JsonValue("Math"));
    courses.push_back(JsonValue("Physics"));
    obj["courses"] = JsonValue(courses);

    JsonObject address;
    address["street"] = JsonValue("123 Main St");
    address["city"] = JsonValue("Anytown");
    obj["address"] = JsonValue(address);

    JsonValue value(obj);
    std::string json_str = value.to_string();

    // Parse it back to verify
    JsonValue parsed_value = parse_json(json_str);
    EXPECT_TRUE(parsed_value.is_object());

    const JsonObject& parsed_obj = parsed_value.as_object();
    EXPECT_EQ(parsed_obj.at("name").as_string(), "John Doe");
    EXPECT_DOUBLE_EQ(parsed_obj.at("age").as_number(), 30);
    EXPECT_FALSE(parsed_obj.at("isStudent").as_bool());
}

TEST_F(JsonTest, HandlesEscapedCharacters) {
    std::string json_str = "\"Hello\
World\"";
    JsonValue value = parse_json(json_str);
    EXPECT_TRUE(value.is_string());
    EXPECT_EQ(value.as_string(), "Hello
World");
}

TEST_F(JsonTest, ParsesFromFile) {
    // Create a test JSON file
    auto test_file = test_dir / "test.json";
    std::ofstream file(test_file);
    file << "{\"key\": \"value\"}";
    file.close();

    // Parse the file
    JsonValue value = parse_json_file(test_file);
    EXPECT_TRUE(value.is_object());
    const JsonObject& obj = value.as_object();
    EXPECT_EQ(obj.size(), 1);
    EXPECT_EQ(obj.at("key").as_string(), "value");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
