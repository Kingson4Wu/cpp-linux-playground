---
sidebar_position: 3
---

# JSON Parser Implementation

Build a lightweight, high-performance JSON parser library that can parse JSON strings and provide convenient access to their values. This project introduces you to recursive descent parsing and object-oriented design in C++.

## Project Structure

The JSON parser is organized as follows:

```
phase1/json-parser/
├── CMakeLists.txt
├── include/
│   └── json.h    # JSON parser interface and classes
└── src/
    └── json_parser.cpp    # Implementation of JSON parser logic
```

## Learning Objectives

During this project, you will:

- Implement a recursive descent parser for JSON
- Design a flexible class hierarchy for representing JSON values
- Handle various data types (objects, arrays, strings, numbers, booleans, null)
- Practice exception handling for malformed JSON
- Apply C++20 features like concepts and ranges

## Core Data Structures

### JsonValue Base Class

The parser should implement a polymorphic approach using a base class for all JSON values:

```cpp
// Example: JsonValue base class structure
class JsonValue {
public:
    virtual ~JsonValue() = default;
    virtual std::string type_name() const = 0;
    virtual std::string stringify() const = 0;
};

class JsonObject : public JsonValue {
    std::map<std::string, std::unique_ptr<JsonValue>> values;
public:
    // Methods to access object properties
};

class JsonArray : public JsonValue {
    std::vector<std::unique_ptr<JsonValue>> elements;
public:
    // Methods to access array elements
};
```

### Parser Interface

Design a clean interface for the parser:

```cpp
namespace json {
    std::unique_ptr<JsonValue> parse(const std::string& json_str);
    std::unique_ptr<JsonValue> parse_file(const std::string& filepath);
}
```

## Implementation Requirements

### Core Parsing Features

1. **Value Types**: Support all JSON value types:
   - Objects (key-value pairs)
   - Arrays (ordered lists)
   - Strings (with escape sequence handling)
   - Numbers (integers and floating-point)
   - Boolean values (`true`/`false`)
   - Null value

2. **String Handling**: Properly handle escaped characters in JSON strings
   - Backslash escapes (`\\`, `\"`, `\/`)
   - Unicode escapes (`\uXXXX`)
   - Control characters (`\n`, `\t`, `\r`)

3. **Error Reporting**: Provide meaningful error messages for invalid JSON
   - Mismatched brackets or braces
   - Invalid escape sequences
   - Unclosed strings
   - Missing commas or colons

### Sample Implementation Approach

```cpp
class JsonParser {
private:
    std::string input;
    size_t pos;
    
    char current_char() const;
    void advance();
    void skip_whitespace();
    
public:
    JsonParser(const std::string& input);
    std::unique_ptr<JsonValue> parse();
    
private:
    std::unique_ptr<JsonValue> parse_value();
    std::unique_ptr<JsonObject> parse_object();
    std::unique_ptr<JsonArray> parse_array();
    std::string parse_string();
    double parse_number();
    bool parse_boolean();
    void parse_null();
};
```

## Usage Examples

Once complete, your JSON parser should support:

```cpp
#include "json.h"

// Parse a JSON string
auto json_val = json::parse(R"({
    "name": "John Doe",
    "age": 30,
    "active": true,
    "scores": [85, 92, 78],
    "address": {
        "street": "123 Main St",
        "city": "Anytown"
    }
})");

// Access values (implementation dependent)
// auto name = json_val->get_object()["name"]->as_string();
```

## Performance Considerations

- Minimize dynamic memory allocations during parsing
- Consider using string views for efficient string handling
- Optimize for common JSON patterns

## Testing Strategy

Write comprehensive tests covering:

- Valid JSON input of various complexity
- Malformed JSON with appropriate error handling
- Edge cases like empty objects/arrays
- All supported data types
- Nested structures

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# Run specific tests for JSON parser (if implemented)
./tests/phase1/json-parser/json_parser_tests
```

## Next Steps

Complete the [Multi-threaded Logger](./logger.md) project next to explore concurrent programming concepts.