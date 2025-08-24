#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <optional>

namespace redis {

// RESP data types
enum class RESPType {
    SIMPLE_STRING,
    ERROR,
    INTEGER,
    BULK_STRING,
    ARRAY
};

// Base class for RESP values
struct RESPValue {
    RESPType type;
    virtual ~RESPValue() = default;
};

// Simple string: +OK\r\n
struct SimpleString : RESPValue {
    std::string value;

    SimpleString(const std::string& val) : value(val) {
        type = RESPType::SIMPLE_STRING;
    }
};

// Error: -Error message\r\n
struct Error : RESPValue {
    std::string message;

    Error(const std::string& msg) : message(msg) {
        type = RESPType::ERROR;
    }
};

// Integer: :1000\r\n
struct Integer : RESPValue {
    long long value;

    Integer(long long val) : value(val) {
        type = RESPType::INTEGER;
    }
};

// Bulk string: $5\r\nhello\r\n or $-1\r\n (null)
struct BulkString : RESPValue {
    std::optional<std::string> value;

    BulkString(const std::string& val) : value(val) {
        type = RESPType::BULK_STRING;
    }

    BulkString() : value(std::nullopt) {
        type = RESPType::BULK_STRING;
    }
};

// Array: *2\r\n$5\r\nhello\r\n$5\r\nworld\r\n
struct Array : RESPValue {
    std::vector<std::unique_ptr<RESPValue>> elements;
    bool is_null;

    Array() : is_null(false) {
        type = RESPType::ARRAY;
    }

    void AddElement(std::unique_ptr<RESPValue> element) {
        elements.push_back(std::move(element));
    }

    bool IsNull() const {
        return is_null;
    }

    void SetNull(bool null = true) {
        is_null = null;
    }
};

/**
 * @brief Parser for Redis Serialization Protocol (RESP)
 * 
 * This class handles parsing and serialization of Redis commands and responses
 * according to the RESP protocol specification.
 */
class Protocol {
public:
    Protocol() = default;
    ~Protocol() = default;

    // Delete copy constructor and assignment operator
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    /**
     * @brief Parse a RESP message from a string
     * @param data The RESP-encoded data
     * @return A unique pointer to the parsed RESPValue, or nullptr on error
     */
    std::unique_ptr<RESPValue> Parse(const std::string& data);

    /**
     * @brief Serialize a RESPValue to a string
     * @param value The RESPValue to serialize
     * @return The RESP-encoded string
     */
    std::string Serialize(const RESPValue& value);

    /**
     * @brief Create a simple string value
     * @param value The string value
     * @return A unique pointer to the SimpleString
     */
    std::unique_ptr<SimpleString> CreateSimpleString(const std::string& value);

    /**
     * @brief Create an error value
     * @param message The error message
     * @return A unique pointer to the Error
     */
    std::unique_ptr<Error> CreateError(const std::string& message);

    /**
     * @brief Create an integer value
     * @param value The integer value
     * @return A unique pointer to the Integer
     */
    std::unique_ptr<Integer> CreateInteger(long long value);

    /**
     * @brief Create a bulk string value
     * @param value The string value (can be empty)
     * @return A unique pointer to the BulkString
     */
    std::unique_ptr<BulkString> CreateBulkString(const std::string& value);

    /**
     * @brief Create a null bulk string value
     * @return A unique pointer to the BulkString representing null
     */
    std::unique_ptr<BulkString> CreateNullBulkString();

    /**
     * @brief Create an array value
     * @return A unique pointer to the Array
     */
    std::unique_ptr<Array> CreateArray();

private:
    /**
     * @brief Parse a single RESP value from a string iterator
     * @param it Iterator pointing to the current position in the data
     * @param end Iterator pointing to the end of the data
     * @return A unique pointer to the parsed RESPValue, or nullptr on error
     */
    std::unique_ptr<RESPValue> ParseValue(std::string::const_iterator& it, std::string::const_iterator end);

    /**
     * @brief Parse a simple string (+...)
     * @param it Iterator pointing to the current position in the data
     * @param end Iterator pointing to the end of the data
     * @return A unique pointer to the parsed SimpleString, or nullptr on error
     */
    std::unique_ptr<RESPValue> ParseSimpleString(std::string::const_iterator& it, std::string::const_iterator end);

    /**
     * @brief Parse an error (-...)
     * @param it Iterator pointing to the current position in the data
     * @param end Iterator pointing to the end of the data
     * @return A unique pointer to the parsed Error, or nullptr on error
     */
    std::unique_ptr<RESPValue> ParseError(std::string::const_iterator& it, std::string::const_iterator end);

    /**
     * @brief Parse an integer (:...)
     * @param it Iterator pointing to the current position in the data
     * @param end Iterator pointing to the end of the data
     * @return A unique pointer to the parsed Integer, or nullptr on error
     */
    std::unique_ptr<RESPValue> ParseInteger(std::string::const_iterator& it, std::string::const_iterator end);

    /**
     * @brief Parse a bulk string ($...)
     * @param it Iterator pointing to the current position in the data
     * @param end Iterator pointing to the end of the data
     * @return A unique pointer to the parsed BulkString, or nullptr on error
     */
    std::unique_ptr<RESPValue> ParseBulkString(std::string::const_iterator& it, std::string::const_iterator end);

    /**
     * @brief Parse an array (*...)
     * @param it Iterator pointing to the current position in the data
     * @param end Iterator pointing to the end of the data
     * @return A unique pointer to the parsed Array, or nullptr on error
     */
    std::unique_ptr<RESPValue> ParseArray(std::string::const_iterator& it, std::string::const_iterator end);
};

} // namespace redis

#endif // PROTOCOL_H