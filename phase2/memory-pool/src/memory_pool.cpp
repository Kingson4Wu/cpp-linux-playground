#include "memory_pool.h"
#include <cstdlib>
#include <new>
#include <iostream>
#include <algorithm>
#include <cassert>

namespace memory_pool {

MemoryPool::MemoryPool(size_t initial_pool_size)
    : total_size_(initial_pool_size), used_size_(0) {
    // Allocate memory pool
    memory_pool_ = std::malloc(total_size_);
    if (!memory_pool_) {
        throw std::bad_alloc();
    }

    // Initially, the entire pool is one free block
    free_blocks_[total_size_] = memory_pool_;
}

MemoryPool::~MemoryPool() {
    std::free(memory_pool_);
}

void* MemoryPool::Allocate(size_t size) {
    if (size == 0) {
        return nullptr;
    }

    // Find the smallest free block that can accommodate the requested size
    auto it = free_blocks_.lower_bound(size);
    if (it == free_blocks_.end()) {
        // No suitable block found
        return nullptr;
    }

    // Get the block
    size_t block_size = it->first;
    void* block_ptr = it->second;
    free_blocks_.erase(it);

    // If the block is larger than needed, split it
    size_t remaining_size = block_size - size;
    if (remaining_size > 0) {
        // Add the remaining part back to free blocks
        free_blocks_[remaining_size] = static_cast<char*>(block_ptr) + size;
    }

    used_size_ += size;
    return block_ptr;
}

void MemoryPool::Deallocate(void* ptr, size_t size) {
    if (!ptr || size == 0) {
        return;
    }

    // Check if the pointer is within the pool
    char* pool_start = static_cast<char*>(memory_pool_);
    char* pool_end = pool_start + total_size_;
    char* ptr_char = static_cast<char*>(ptr);

    if (ptr_char < pool_start || ptr_char + size > pool_end) {
        // Pointer is not from this pool
        return;
    }

    // Add the block back to free blocks
    free_blocks_[size] = ptr;
    used_size_ -= size;

    // Coalesce adjacent free blocks (simplified)
    // This is a basic implementation and might not handle all cases
    // A more sophisticated implementation would be needed for production use
}

} // namespace memory_pool
