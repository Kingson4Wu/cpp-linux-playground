/*
 * json.cpp - Implementation of JSON parser and JsonValue class
 *
 * This file contains the core implementation of a simple JSON parser
 * and the JsonValue class used by the JSON parser.
 *
 * How to Compile with Docker:
 *   1. Build the project in Docker container:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run Tests:
 *   1. After building, run: ./build/phase1/json-parser/json_test
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "json_test") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "json.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <limits>

// Constructors
JsonValue::JsonValue() : value_(nullptr) {}

JsonValue::JsonValue(std::nullptr_t) : value_(nullptr) {}

JsonValue::JsonValue(bool value) : value_(value) {}

JsonValue::JsonValue(int value) : value_(static_cast<double>(value)) {}

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

// Destructor
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

// Value accessors
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
    return std::get<JsonArray>(value_)[index];
}

const JsonValue& JsonValue::operator[](size_t index) const {
    return std::get<JsonArray>(value_)[index];
}

JsonValue& JsonValue::operator[](const std::string& key) {
    return std::get<JsonObject>(value_)[key];
}

const JsonValue& JsonValue::operator[](const std::string& key) const {
    return std::get<JsonObject>(value_).at(key);
}

// Array methods
void JsonValue::push_back(const JsonValue& value) {
    std::get<JsonArray>(value_).push_back(value);
}

void JsonValue::push_back(JsonValue&& value) {
    std::get<JsonArray>(value_).push_back(std::move(value));
}

// Object methods
void JsonValue::insert(const std::string& key, const JsonValue& value) {
    std::get<JsonObject>(value_)[key] = value;
}

void JsonValue::insert(const std::string& key, JsonValue&& value) {
    std::get<JsonObject>(value_)[key] = std::move(value);
}

// Serialization
std::string JsonValue::to_string() const {
    return serialize_value();
}

std::string JsonValue::serialize_value() const {
    if (is_null()) {
        return "null";
    } else if (is_bool()) {
        return as_bool() ? "true" : "false";
    } else if (is_number()) {
        double num = as_number();
        if (std::isinf(num) || std::isnan(num)) {
            return "null"; // JSON doesn't support infinity or NaN
        }
        // Format number to remove trailing zeros
        std::ostringstream oss;
        oss << std::setprecision(std::numeric_limits<double>::digits10) << num;
        return oss.str();
    } else if (is_string()) {
        // Escape special characters in string
        const std::string& str = as_string();
        std::string result = "\"";
        for (char c : str) {
            switch (c) {
                case '"':  result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\b': result += "\\b";  break;
                case '\f': result += "\\f";  break;
                case '
': result += "\
";  break;
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
            }
        }
        result += "\"";
        return result;
    } else if (is_array()) {
        const JsonArray& arr = as_array();
        std::string result = "[";
        for (size_t i = 0; i < arr.size(); ++i) {
            if (i > 0) result += ",";
            result += arr[i].serialize_value();
        }
        result += "]";
        return result;
    } else if (is_object()) {
        const JsonObject& obj = as_object();
        std::string result = "{";
        bool first = true;
        for (const auto& pair : obj) {
            if (!first) result += ",";
            first = false;
            result += "\"" + pair.first + "\":" + pair.second.serialize_value();
        }
        result += "}";
        return result;
    }
    return "null"; // Should not happen
}

// Parser implementation
namespace {
    // Helper class for parsing
    class JsonParser {
    public:
        JsonParser(const std::string& str) : str_(str), pos_(0) {}

        JsonValue parse() {
            skip_whitespace();
            JsonValue result = parse_value();
            skip_whitespace();
            if (pos_ < str_.size()) {
                throw std::runtime_error("Unexpected character after JSON value");
            }
            return result;
        }

    private:
        const std::string& str_;
        size_t pos_;

        void skip_whitespace() {
            while (pos_ < str_.size() && std::isspace(str_[pos_])) {
                ++pos_;
            }
        }

        char peek() const {
            if (pos_ >= str_.size()) {
                throw std::runtime_error("Unexpected end of input");
            }
            return str_[pos_];
        }

        char get() {
            if (pos_ >= str_.size()) {
                throw std::runtime_error("Unexpected end of input");
            }
            return str_[pos_++];
        }

        void expect(char c) {
            if (get() != c) {
                throw std::runtime_error("Expected '" + std::string(1, c) + "'");
            }
        }

        JsonValue parse_value() {
            skip_whitespace();
            if (pos_ >= str_.size()) {
                throw std::runtime_error("Unexpected end of input");
            }

            char c = peek();
            switch (c) {
                case 'n': return parse_null();
                case 't': return parse_true();
                case 'f': return parse_false();
                case '"': return parse_string();
                case '[': return parse_array();
                case '{': return parse_object();
                case '-':
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    return parse_number();
                default:
                    throw std::runtime_error("Invalid JSON value");
            }
        }

        JsonValue parse_null() {
            if (pos_ + 4 <= str_.size() && str_.substr(pos_, 4) == "null") {
                pos_ += 4;
                return JsonValue(nullptr);
            }
            throw std::runtime_error("Invalid null value");
        }

        JsonValue parse_true() {
            if (pos_ + 4 <= str_.size() && str_.substr(pos_, 4) == "true") {
                pos_ += 4;
                return JsonValue(true);
            }
            throw std::runtime_error("Invalid true value");
        }

        JsonValue parse_false() {
            if (pos_ + 5 <= str_.size() && str_.substr(pos_, 5) == "false") {
                pos_ += 5;
                return JsonValue(false);
            }
            throw std::runtime_error("Invalid false value");
        }

        JsonValue parse_number() {
            size_t start = pos_;
            if (peek() == '-') {
                get();
            }

            // Parse integer part
            if (peek() == '0') {
                get();
            } else if (std::isdigit(peek())) {
                while (pos_ < str_.size() && std::isdigit(str_[pos_])) {
                    get();
                }
            } else {
                throw std::runtime_error("Invalid number");
            }

            // Parse fractional part
            if (pos_ < str_.size() && peek() == '.') {
                get();
                if (pos_ >= str_.size() || !std::isdigit(peek())) {
                    throw std::runtime_error("Invalid number");
                }
                while (pos_ < str_.size() && std::isdigit(str_[pos_])) {
                    get();
                }
            }

            // Parse exponent part
            if (pos_ < str_.size() && (peek() == 'e' || peek() == 'E')) {
                get();
                if (pos_ < str_.size() && (peek() == '+' || peek() == '-')) {
                    get();
                }
                if (pos_ >= str_.size() || !std::isdigit(peek())) {
                    throw std::runtime_error("Invalid number");
                }
                while (pos_ < str_.size() && std::isdigit(str_[pos_])) {
                    get();
                }
            }

            std::string num_str = str_.substr(start, pos_ - start);
            try {
                return JsonValue(std::stod(num_str));
            } catch (const std::exception&) {
                throw std::runtime_error("Invalid number");
            }
        }

        JsonValue parse_string() {
            expect('"');
            std::string result;
            while (pos_ < str_.size() && peek() != '"') {
                char c = get();
                if (c == '\\') {
                    c = get();
                    switch (c) {
                        case '"':  result += '"';  break;
                        case '\\': result += '\\'; break;
                        case '/':  result += '/';  break;
                        case 'b':  result += '\b'; break;
                        case 'f':  result += '\f'; break;
                        case 'n':  result += '
'; break;
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
                            throw std::runtime_error("Invalid escape sequence");
                    }
                } else {
                    result += c;
                }
            }
            expect('"');
            return JsonValue(result);
        }

        JsonValue parse_array() {
            expect('[');
            skip_whitespace();

            JsonArray result;
            if (peek() == ']') {
                get(); // Consume ']'
                return JsonValue(result);
            }

            result.push_back(parse_value());
            skip_whitespace();

            while (peek() == ',') {
                get(); // Consume ','
                skip_whitespace();
                result.push_back(parse_value());
                skip_whitespace();
            }

            expect(']');
            return JsonValue(result);
        }

        JsonValue parse_object() {
            expect('{');
            skip_whitespace();

            JsonObject result;
            if (peek() == '}') {
                get(); // Consume '}'
                return JsonValue(result);
            }

            auto pair = parse_pair();
            result.insert(pair);
            skip_whitespace();

            while (peek() == ',') {
                get(); // Consume ','
                skip_whitespace();
                pair = parse_pair();
                result.insert(pair);
                skip_whitespace();
            }

            expect('}');
            return JsonValue(result);
        }

        std::pair<std::string, JsonValue> parse_pair() {
            std::string key = parse_string().as_string();
            skip_whitespace();
            expect(':');
            skip_whitespace();
            JsonValue value = parse_value();
            return {key, value};
        }
    };
}

// Parse a JSON string into a JsonValue
JsonValue parse_json(const std::string& json_str) {
    JsonParser parser(json_str);
    return parser.parse();
}

// Parse a JSON file into a JsonValue
JsonValue parse_json_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    return parse_json(content);
}
