#include "fixed_block_allocator.h"
#include <cstdlib>
#include <new>
#include <iostream>
#include <algorithm>
#include <cassert>

namespace memory_pool {

FixedBlockAllocator::FixedBlockAllocator(size_t block_size, size_t num_blocks)
    : block_size_(block_size), num_blocks_(num_blocks), num_free_blocks_(num_blocks) {
    // Allocate memory pool with alignment
    size_t total_size = block_size_ * num_blocks_;
    memory_pool_ = std::aligned_alloc(block_size_, total_size);
    if (!memory_pool_) {
        throw std::bad_alloc();
    }

    // Initialize free list
    free_list_ = static_cast<void**>(std::malloc(num_blocks_ * sizeof(void*)));
    if (!free_list_) {
        std::free(memory_pool_);
        throw std::bad_alloc();
    }

    // Populate free list with pointers to each block
    char* pool_ptr = static_cast<char*>(memory_pool_);
    for (size_t i = 0; i < num_blocks_; ++i) {
        free_list_[i] = pool_ptr + i * block_size_;
    }
}

FixedBlockAllocator::~FixedBlockAllocator() {
    std::free(free_list_);
    std::free(memory_pool_);
}

void* FixedBlockAllocator::Allocate() {
    if (num_free_blocks_ == 0) {
        return nullptr; // No free blocks available
    }

    // Get a block from the free list
    void* block = free_list_[--num_free_blocks_];
    return block;
}

void FixedBlockAllocator::Deallocate(void* ptr) {
    if (!ptr) {
        return; // Cannot deallocate null pointer
    }

    // Check if the pointer is within the pool
    char* pool_start = static_cast<char*>(memory_pool_);
    char* pool_end = pool_start + (block_size_ * num_blocks_);
    char* ptr_char = static_cast<char*>(ptr);

    if (ptr_char < pool_start || ptr_char >= pool_end) {
        // Pointer is not from this pool
        return;
    }

    // Check if the pointer is properly aligned
    size_t offset = ptr_char - pool_start;
    if (offset % block_size_ != 0) {
        // Pointer is not aligned to a block boundary
        return;
    }

    // Add the block back to the free list
    free_list_[num_free_blocks_++] = ptr;
}

} // namespace memory_pool
