/*
 * json_example.cpp - Example usage of the JSON parser
 *
 * This file demonstrates how to use the JSON parser library.
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
 * How to Run:
 *   ./build/phase1/json-parser/json_example
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "json_example") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "json.h"
#include <iostream>
#include <string>

int main() {
    // Parse a JSON string
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

    try {
        JsonValue value = parse_json(json_str);

        // Access values
        const JsonObject& obj = value.as_object();
        std::cout << "Name: " << obj.at("name").as_string() << std::endl;
        std::cout << "Age: " << obj.at("age").as_number() << std::endl;
        std::cout << "Is Student: " << (obj.at("isStudent").as_bool() ? "Yes" : "No") << std::endl;

        // Access array
        const JsonArray& courses = obj.at("courses").as_array();
        std::cout << "Courses: ";
        for (size_t i = 0; i < courses.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << courses[i].as_string();
        }
        std::cout << std::endl;

        // Access nested object
        const JsonObject& address = obj.at("address").as_object();
        std::cout << "Address: " << address.at("street").as_string() 
                  << ", " << address.at("city").as_string() << std::endl;

        // Serialize back to JSON string
        std::string serialized = value.to_string();
        std::cout << "Serialized JSON: " << serialized << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
