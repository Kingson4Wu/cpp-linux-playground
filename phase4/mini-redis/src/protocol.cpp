#include "protocol.h"
#include <sstream>
#include <stdexcept>
#include <charconv>

namespace redis {

std::unique_ptr<RESPValue> Protocol::Parse(const std::string& data) {
    if (data.empty()) {
        return nullptr;
    }

    auto it = data.begin();
    return ParseValue(it, data.end());
}

std::unique_ptr<RESPValue> Protocol::ParseValue(std::string::const_iterator& it, std::string::const_iterator end) {
    if (it == end) {
        return nullptr;
    }

    char prefix = *it++;

    switch (prefix) {
        case '+':
            return ParseSimpleString(it, end);
        case '-':
            return ParseError(it, end);
        case ':':
            return ParseInteger(it, end);
        case '$':
            return ParseBulkString(it, end);
        case '*':
            return ParseArray(it, end);
        default:
            return nullptr;
    }
}

std::unique_ptr<RESPValue> Protocol::ParseSimpleString(std::string::const_iterator& it, std::string::const_iterator end) {
    auto start = it;
    while (it != end && *it != '\r') {
        ++it;
    }

    if (it == end || (it + 1) == end || *(it + 1) != '
') {
        return nullptr; // Invalid format
    }

    std::string value(start, it);
    it += 2; // Skip \r

    return std::make_unique<SimpleString>(value);
}

std::unique_ptr<RESPValue> Protocol::ParseError(std::string::const_iterator& it, std::string::const_iterator end) {
    auto start = it;
    while (it != end && *it != '\r') {
        ++it;
    }

    if (it == end || (it + 1) == end || *(it + 1) != '
') {
        return nullptr; // Invalid format
    }

    std::string message(start, it);
    it += 2; // Skip \r

    return std::make_unique<Error>(message);
}

std::unique_ptr<RESPValue> Protocol::ParseInteger(std::string::const_iterator& it, std::string::const_iterator end) {
    auto start = it;
    while (it != end && *it != '\r') {
        ++it;
    }

    if (it == end || (it + 1) == end || *(it + 1) != '
') {
        return nullptr; // Invalid format
    }

    std::string number_str(start, it);
    it += 2; // Skip \r

    try {
        long long value = std::stoll(number_str);
        return std::make_unique<Integer>(value);
    } catch (const std::exception&) {
        return nullptr; // Invalid number format
    }
}

std::unique_ptr<RESPValue> Protocol::ParseBulkString(std::string::const_iterator& it, std::string::const_iterator end) {
    // Parse the length
    auto start = it;
    while (it != end && *it != '\r') {
        ++it;
    }

    if (it == end || (it + 1) == end || *(it + 1) != '
') {
        return nullptr; // Invalid format
    }

    std::string length_str(start, it);
    it += 2; // Skip \r

    try {
        long long length = std::stoll(length_str);

        // Handle null bulk string
        if (length == -1) {
            return std::make_unique<BulkString>();
        }

        // Check if we have enough data
        if (std::distance(it, end) < length + 2) { // +2 for \r

            return nullptr; // Not enough data
        }

        std::string value(it, it + length);
        it += length + 2; // Skip value and \r

        return std::make_unique<BulkString>(value);
    } catch (const std::exception&) {
        return nullptr; // Invalid number format
    }
}

std::unique_ptr<RESPValue> Protocol::ParseArray(std::string::const_iterator& it, std::string::const_iterator end) {
    // Parse the length
    auto start = it;
    while (it != end && *it != '\r') {
        ++it;
    }

    if (it == end || (it + 1) == end || *(it + 1) != '
') {
        return nullptr; // Invalid format
    }

    std::string length_str(start, it);
    it += 2; // Skip \r

    try {
        long long length = std::stoll(length_str);

        // Handle null array
        if (length == -1) {
            return std::make_unique<Array>();
        }

        auto array = std::make_unique<Array>();

        // Parse elements
        for (long long i = 0; i < length; ++i) {
            auto element = ParseValue(it, end);
            if (!element) {
                return nullptr; // Failed to parse element
            }
            array->AddElement(std::move(element));
        }

        return array;
    } catch (const std::exception&) {
        return nullptr; // Invalid number format
    }
}

std::string Protocol::Serialize(const RESPValue& value) {
    std::ostringstream oss;

    switch (value.type) {
        case RESPType::SIMPLE_STRING: {
            const auto& simple_string = static_cast<const SimpleString&>(value);
            oss << "+" << simple_string.value << "\r
";
            break;
        }
        case RESPType::ERROR: {
            const auto& error = static_cast<const Error&>(value);
            oss << "-" << error.message << "\r
";
            break;
        }
        case RESPType::INTEGER: {
            const auto& integer = static_cast<const Integer&>(value);
            oss << ":" << integer.value << "\r
";
            break;
        }
        case RESPType::BULK_STRING: {
            const auto& bulk_string = static_cast<const BulkString&>(value);
            if (!bulk_string.value.has_value()) {
                oss << "$-1\r
";
            } else {
                oss << "$" << bulk_string.value->size() << "\r
" << bulk_string.value.value() << "\r
";
            }
            break;
        }
        case RESPType::ARRAY: {
            const auto& array = static_cast<const Array&>(value);
            oss << "*" << array.elements.size() << "\r
";
            for (const auto& element : array.elements) {
                oss << Serialize(*element);
            }
            break;
        }
    }

    return oss.str();
}

std::unique_ptr<SimpleString> Protocol::CreateSimpleString(const std::string& value) {
    return std::make_unique<SimpleString>(value);
}

std::unique_ptr<Error> Protocol::CreateError(const std::string& message) {
    return std::make_unique<Error>(message);
}

std::unique_ptr<Integer> Protocol::CreateInteger(long long value) {
    return std::make_unique<Integer>(value);
}

std::unique_ptr<BulkString> Protocol::CreateBulkString(const std::string& value) {
    return std::make_unique<BulkString>(value);
}

std::unique_ptr<BulkString> Protocol::CreateNullBulkString() {
    return std::make_unique<BulkString>();
}

std::unique_ptr<Array> Protocol::CreateArray() {
    return std::make_unique<Array>();
}

} // namespace redis
