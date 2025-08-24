#include "json.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <fstream>

// Implementation of JsonValue methods
namespace {

// Helper function to escape special characters in a string for JSON output
std::string EscapeString(const std::string& str) {
    std::string result = "\"";
    for (char c : str) {
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b";  break;
            case '\f': result += "\\f";  break;
            case '\n': result += "\\n";  break;
            case '\r': result += "\\r";  break;
            case '\t': result += "\\t";  break;
            default:
                if (c >= 0 && c < 0x20) {
                    // Escape control characters
                    std::ostringstream oss;
                    oss << "\\u00" << std::hex << std::setw(2) << std::setfill('0') << (int)c;
                    result += oss.str();
                } else {
                    result += c;
                }
                break;
        }
    }
    result += "\"";
    return result;
}

// Helper function to trim whitespace from a string
std::string Trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return ""; // String contains only whitespace
    }
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

} // anonymous namespace

// Constructor implementations for different JSON value types
JsonValue::JsonValue() : value_(nullptr) {}

JsonValue::JsonValue(std::nullptr_t) : value_(nullptr) {}

JsonValue::JsonValue(bool value) : value_(value) {}

JsonValue::JsonValue(int value) : value_((double)value) {}

JsonValue::JsonValue(double value) : value_(value) {}

JsonValue::JsonValue(const std::string& value) : value_(value) {}

JsonValue::JsonValue(const char* value) : value_(std::string(value)) {}

JsonValue::JsonValue(const JsonArray& value) : value_(value) {}

JsonValue::JsonValue(const JsonObject& value) : value_(value) {}

// Copy and move constructors/operators
JsonValue::JsonValue(const JsonValue& other) : value_(other.value_) {}

JsonValue::JsonValue(JsonValue&& other) noexcept : value_(std::move(other.value_)) {}

JsonValue& JsonValue::operator=(const JsonValue& other) {
    if (this != &other) {
        value_ = other.value_;
    }
    return *this;
}

JsonValue& JsonValue::operator=(JsonValue&& other) noexcept {
    if (this != &other) {
        value_ = std::move(other.value_);
    }
    return *this;
}

JsonValue::~JsonValue() = default;

// Type checking
bool JsonValue::is_null() const {
    return std::holds_alternative<std::nullptr_t>(value_);
}

bool JsonValue::is_bool() const {
    return std::holds_alternative<bool>(value_);
}

bool JsonValue::is_number() const {
    return std::holds_alternative<double>(value_);
}

bool JsonValue::is_string() const {
    return std::holds_alternative<std::string>(value_);
}

bool JsonValue::is_array() const {
    return std::holds_alternative<JsonArray>(value_);
}

bool JsonValue::is_object() const {
    return std::holds_alternative<JsonObject>(value_);
}

// Value accessors - will throw std::bad_variant_access if type doesn't match
bool JsonValue::as_bool() const {
    return std::get<bool>(value_);
}

double JsonValue::as_number() const {
    return std::get<double>(value_);
}

const std::string& JsonValue::as_string() const {
    return std::get<std::string>(value_);
}

const JsonArray& JsonValue::as_array() const {
    return std::get<JsonArray>(value_);
}

const JsonObject& JsonValue::as_object() const {
    return std::get<JsonObject>(value_);
}

// Array/Object element access
JsonValue& JsonValue::operator[](size_t index) {
    JsonArray& arr = std::get<JsonArray>(value_);
    if (index >= arr.size()) {
        throw std::out_of_range("Index out of range");
    }
    return arr[index];
}

const JsonValue& JsonValue::operator[](size_t index) const {
    const JsonArray& arr = std::get<JsonArray>(value_);
    if (index >= arr.size()) {
        throw std::out_of_range("Index out of range");
    }
    return arr[index];
}

JsonValue& JsonValue::operator[](const std::string& key) {
    JsonObject& obj = std::get<JsonObject>(value_);
    return obj[key];
}

const JsonValue& JsonValue::operator[](const std::string& key) const {
    const JsonObject& obj = std::get<JsonObject>(value_);
    auto it = obj.find(key);
    if (it == obj.end()) {
        throw std::out_of_range("Key not found");
    }
    return it->second;
}

// Array methods
void JsonValue::push_back(const JsonValue& value) {
    JsonArray& arr = std::get<JsonArray>(value_);
    arr.push_back(value);
}

void JsonValue::push_back(JsonValue&& value) {
    JsonArray& arr = std::get<JsonArray>(value_);
    arr.push_back(std::move(value));
}

// Object methods
void JsonValue::insert(const std::string& key, const JsonValue& value) {
    JsonObject& obj = std::get<JsonObject>(value_);
    obj[key] = value;
}

void JsonValue::insert(const std::string& key, JsonValue&& value) {
    JsonObject& obj = std::get<JsonObject>(value_);
    obj[key] = std::move(value);
}

// Serialization helper
std::string JsonValue::serialize_value() const {
    if (std::holds_alternative<std::nullptr_t>(value_)) {
        return "null";
    } else if (std::holds_alternative<bool>(value_)) {
        return std::get<bool>(value_) ? "true" : "false";
    } else if (std::holds_alternative<double>(value_)) {
        double num = std::get<double>(value_);
        // Handle special cases for infinity and NaN
        if (std::isinf(num)) {
            throw std::runtime_error("Infinity values are not allowed in JSON");
        }
        if (std::isnan(num)) {
            throw std::runtime_error("NaN values are not allowed in JSON");
        }
        
        // Format the number to avoid unnecessary decimal points
        if (num == std::floor(num) && 
            std::abs(num) < std::pow(10.0, 15)) {
            // It's an integer value within the safe range
            return std::to_string((long long)num);
        } else {
            // It's a floating-point value
            std::ostringstream oss;
            oss << std::setprecision(std::numeric_limits<double>::digits10) << num;
            return oss.str();
        }
    } else if (std::holds_alternative<std::string>(value_)) {
        return EscapeString(std::get<std::string>(value_));
    } else if (std::holds_alternative<JsonArray>(value_)) {
        const JsonArray& arr = std::get<JsonArray>(value_);
        std::string result = "[";
        bool first = true;
        for (const auto& element : arr) {
            if (!first) {
                result += ",";
            }
            first = false;
            result += element.serialize_value();
        }
        result += "]";
        return result;
    } else if (std::holds_alternative<JsonObject>(value_)) {
        const JsonObject& obj = std::get<JsonObject>(value_);
        std::string result = "{";
        bool first = true;
        for (const auto& pair : obj) {
            if (!first) {
                result += ",";
            }
            first = false;
            result += EscapeString(pair.first) + ":" + pair.second.serialize_value();
        }
        result += "}";
        return result;
    }
    
    return "null"; // Should never reach here
}

// Serialization to JSON string
std::string JsonValue::to_string() const {
    return serialize_value();
}

// Parser implementation
namespace json_parser {

class JsonParser {
public:
    JsonParser(const std::string& str) : str_(str), pos_(0) {}

    JsonValue Parse() {
        SkipWhitespace();
        JsonValue result = ParseValue();
        SkipWhitespace();
        
        // Ensure we've consumed the entire input
        if (pos_ < str_.size()) {
            throw std::runtime_error("Unexpected characters at end of input");
        }
        
        return result;
    }

private:
    std::string str_;
    size_t pos_;

    void SkipWhitespace() {
        while (pos_ < str_.size() && std::isspace(str_[pos_])) {
            ++pos_;
        }
    }

    JsonValue ParseValue() {
        SkipWhitespace();
        if (pos_ >= str_.size()) {
            throw std::runtime_error("Unexpected end of input");
        }

        char c = str_[pos_];
        switch (c) {
            case '{': return ParseObject();
            case '[': return ParseArray();
            case '"': return ParseString();
            case 't': return ParseTrue();
            case 'f': return ParseFalse();
            case 'n': return ParseNull();
            default:
                if (c == '-' || std::isdigit(c)) {
                    return ParseNumber();
                } else {
                    throw std::runtime_error("Unexpected character: " + std::string(1, c));
                }
        }
    }

    JsonValue ParseObject() {
        if (str_[pos_] != '{') {
            throw std::runtime_error("Expected '{'");
        }
        ++pos_;
        SkipWhitespace();

        JsonObject obj;
        if (pos_ < str_.size() && str_[pos_] == '}') {
            // Empty object
            ++pos_;
            return JsonValue(obj);
        }

        while (true) {
            SkipWhitespace();
            if (pos_ >= str_.size()) {
                throw std::runtime_error("Unexpected end of input while parsing object");
            }

            // Parse key
            if (str_[pos_] != '"') {
                throw std::runtime_error("Expected string key in object");
            }
            std::string key = ParseString().as_string();

            // Parse colon
            SkipWhitespace();
            if (pos_ >= str_.size() || str_[pos_] != ':') {
                throw std::runtime_error("Expected ':' after key in object");
            }
            ++pos_;

            // Parse value
            SkipWhitespace();
            JsonValue value = ParseValue();
            obj[key] = value;

            // Check for comma or end of object
            SkipWhitespace();
            if (pos_ >= str_.size()) {
                throw std::runtime_error("Unexpected end of input while parsing object");
            }
            if (str_[pos_] == '}') {
                ++pos_;
                break;
            } else if (str_[pos_] == ',') {
                ++pos_;
            } else {
                throw std::runtime_error("Expected ',' or '}' in object");
            }
        }

        return JsonValue(obj);
    }

    JsonValue ParseArray() {
        if (str_[pos_] != '[') {
            throw std::runtime_error("Expected '['");
        }
        ++pos_;
        SkipWhitespace();

        JsonArray arr;
        if (pos_ < str_.size() && str_[pos_] == ']') {
            // Empty array
            ++pos_;
            return JsonValue(arr);
        }

        while (true) {
            SkipWhitespace();
            if (pos_ >= str_.size()) {
                throw std::runtime_error("Unexpected end of input while parsing array");
            }

            // Parse value
            JsonValue value = ParseValue();
            arr.push_back(value);

            // Check for comma or end of array
            SkipWhitespace();
            if (pos_ >= str_.size()) {
                throw std::runtime_error("Unexpected end of input while parsing array");
            }
            if (str_[pos_] == ']') {
                ++pos_;
                break;
            } else if (str_[pos_] == ',') {
                ++pos_;
            } else {
                throw std::runtime_error("Expected ',' or ']' in array");
            }
        }

        return JsonValue(arr);
    }

    JsonValue ParseString() {
        if (str_[pos_] != '"') {
            throw std::runtime_error("Expected '\"'");
        }
        ++pos_;

        std::string result;
        while (pos_ < str_.size() && str_[pos_] != '"') {
            char c = str_[pos_];
            if (c == '\\') {
                // Handle escape sequences
                ++pos_;
                if (pos_ >= str_.size()) {
                    throw std::runtime_error("Unexpected end of input in string");
                }
                c = str_[pos_];
                switch (c) {
                    case '"':  result += '"';  break;
                    case '\\': result += '\\'; break;
                    case '/':  result += '/';  break;
                    case 'b':  result += '\b'; break;
                    case 'f':  result += '\f'; break;
                    case 'n':  result += '\n'; break;
                    case 'r':  result += '\r'; break;
                    case 't':  result += '\t'; break;
                    case 'u': {
                        // Unicode escape sequence - for simplicity, we'll just skip it
                        // A full implementation would parse the 4 hex digits and convert to UTF-8
                        if (pos_ + 4 > str_.size()) {
                            throw std::runtime_error("Invalid unicode escape sequence");
                        }
                        pos_ += 4; // Skip the 4 hex digits
                        result += "?"; // Placeholder for unicode character
                        break;
                    }
                    default:
                        throw std::runtime_error("Invalid escape sequence: \\" + std::string(1, c));
                }
            } else {
                result += c;
            }
            ++pos_;
        }

        if (pos_ >= str_.size()) {
            throw std::runtime_error("Unterminated string");
        }
        ++pos_; // Skip closing quote

        return JsonValue(result);
    }

    JsonValue ParseNumber() {
        size_t start = pos_;
        if (str_[pos_] == '-') {
            ++pos_;
        }

        // Parse integer part
        if (pos_ >= str_.size() || !std::isdigit(str_[pos_])) {
            throw std::runtime_error("Invalid number format");
        }
        while (pos_ < str_.size() && std::isdigit(str_[pos_])) {
            ++pos_;
        }

        // Parse fractional part
        if (pos_ < str_.size() && str_[pos_] == '.') {
            ++pos_;
            if (pos_ >= str_.size() || !std::isdigit(str_[pos_])) {
                throw std::runtime_error("Invalid number format");
            }
            while (pos_ < str_.size() && std::isdigit(str_[pos_])) {
                ++pos_;
            }
        }

        // Parse exponent part
        if (pos_ < str_.size() && (str_[pos_] == 'e' || str_[pos_] == 'E')) {
            ++pos_;
            if (pos_ < str_.size() && (str_[pos_] == '+' || str_[pos_] == '-')) {
                ++pos_;
            }
            if (pos_ >= str_.size() || !std::isdigit(str_[pos_])) {
                throw std::runtime_error("Invalid number format");
            }
            while (pos_ < str_.size() && std::isdigit(str_[pos_])) {
                ++pos_;
            }
        }

        std::string numberStr = str_.substr(start, pos_ - start);
        try {
            double value = std::stod(numberStr);
            return JsonValue(value);
        } catch (const std::exception&) {
            throw std::runtime_error("Invalid number format: " + numberStr);
        }
    }

    JsonValue ParseTrue() {
        if (pos_ + 3 < str_.size() && str_.substr(pos_, 4) == "true") {
            pos_ += 4;
            return JsonValue(true);
        }
        throw std::runtime_error("Expected 'true'");
    }

    JsonValue ParseFalse() {
        if (pos_ + 4 < str_.size() && str_.substr(pos_, 5) == "false") {
            pos_ += 5;
            return JsonValue(false);
        }
        throw std::runtime_error("Expected 'false'");
    }

    JsonValue ParseNull() {
        if (pos_ + 3 < str_.size() && str_.substr(pos_, 4) == "null") {
            pos_ += 4;
            return JsonValue(nullptr);
        }
        throw std::runtime_error("Expected 'null'");
    }
};

} // namespace json_parser

// Public parsing function
JsonValue parse_json(const std::string& jsonStr) {
    json_parser::JsonParser parser(jsonStr);
    return parser.Parse();
}

// Parse a JSON file into a JsonValue
JsonValue parse_json_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    
    std::ostringstream oss;
    oss << file.rdbuf();
    std::string content = oss.str();
    
    return parse_json(content);
}