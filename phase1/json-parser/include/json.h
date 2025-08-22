#pragma once

#include <string>
#include <variant>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

// Forward declaration
class JsonValue;

// Type aliases
using JsonArray = std::vector<JsonValue>;
using JsonObject = std::map<std::string, JsonValue>;

// JsonValue type - can be one of the following:
// null, boolean, number, string, array, object
class JsonValue {
public:
    // Types that JsonValue can hold
    using ValueType = std::variant<std::nullptr_t, bool, double, std::string, JsonArray, JsonObject>;

    // Constructors for different types
    JsonValue();
    JsonValue(std::nullptr_t);
    JsonValue(bool value);
    JsonValue(int value);
    JsonValue(double value);
    JsonValue(const std::string& value);
    JsonValue(const char* value);
    JsonValue(const JsonArray& value);
    JsonValue(const JsonObject& value);

    // Copy and move constructors/operators
    JsonValue(const JsonValue& other);
    JsonValue(JsonValue&& other) noexcept;
    JsonValue& operator=(const JsonValue& other);
    JsonValue& operator=(JsonValue&& other) noexcept;

    // Destructor
    ~JsonValue();

    // Type checking
    bool is_null() const;
    bool is_bool() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    // Value accessors - will throw std::bad_variant_access if type doesn't match
    bool as_bool() const;
    double as_number() const;
    const std::string& as_string() const;
    const JsonArray& as_array() const;
    const JsonObject& as_object() const;

    // Array/Object element access
    JsonValue& operator[](size_t index);
    const JsonValue& operator[](size_t index) const;
    JsonValue& operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;

    // Array methods
    void push_back(const JsonValue& value);
    void push_back(JsonValue&& value);

    // Object methods
    void insert(const std::string& key, const JsonValue& value);
    void insert(const std::string& key, JsonValue&& value);

    // Serialization
    std::string to_string() const;

private:
    ValueType value_;

    // Helper for serialization
    std::string serialize_value() const;
};

// Parse a JSON string into a JsonValue
JsonValue parse_json(const std::string& json_str);

// Parse a JSON file into a JsonValue
JsonValue parse_json_file(const std::string& filepath);
