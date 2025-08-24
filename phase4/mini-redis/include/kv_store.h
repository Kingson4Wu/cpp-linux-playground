#ifndef KV_STORE_H
#define KV_STORE_H

#include <string>
#include <unordered_map>
#include <mutex>
#include <optional>

namespace redis {

/**
 * @brief A thread-safe in-memory key-value store
 * 
 * This class implements a simple key-value store that supports
 * basic operations like SET, GET, and DELETE in a thread-safe manner.
 */
class KVStore {
public:
    KVStore() = default;
    ~KVStore() = default;

    // Delete copy constructor and assignment operator
    KVStore(const KVStore&) = delete;
    KVStore& operator=(const KVStore&) = delete;

    /**
     * @brief Set a key-value pair in the store
     * @param key The key to set
     * @param value The value to associate with the key
     * @return true if the operation was successful, false otherwise
     */
    bool Set(const std::string& key, const std::string& value);

    /**
     * @brief Get the value associated with a key
     * @param key The key to look up
     * @return The value associated with the key, or std::nullopt if the key doesn't exist
     */
    std::optional<std::string> Get(const std::string& key) const;

    /**
     * @brief Delete a key from the store
     * @param key The key to delete
     * @return true if the key existed and was deleted, false if the key didn't exist
     */
    bool Delete(const std::string& key);

    /**
     * @brief Check if a key exists in the store
     * @param key The key to check
     * @return true if the key exists, false otherwise
     */
    bool Exists(const std::string& key) const;

    /**
     * @brief Get the number of key-value pairs in the store
     * @return The number of key-value pairs
     */
    size_t Size() const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::string> store_;
};

} // namespace redis

#endif // KV_STORE_H
