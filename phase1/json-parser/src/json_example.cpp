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
