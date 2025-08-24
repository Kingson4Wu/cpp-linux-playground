#include "kv_store.h"
#include <mutex>

namespace redis {

bool KVStore::Set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    store_[key] = value;
    return true;
}

std::optional<std::string> KVStore::Get(const std::string& key) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = store_.find(key);
    if (it != store_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool KVStore::Delete(const std::string& key) {
    std::lock_guard<std::mutex> lock(mutex_);
    return store_.erase(key) > 0;
}

bool KVStore::Exists(const std::string& key) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return store_.find(key) != store_.end();
}

size_t KVStore::Size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return store_.size();
}

} // namespace redis
