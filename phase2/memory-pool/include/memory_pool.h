#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstddef>
#include <memory>
#include <vector>
#include <map>

namespace memory_pool {

/**
 * @brief A general-purpose memory pool allocator.
 * 
 * This allocator manages a pool of memory and can allocate blocks of
 * different sizes. It uses a simple segregated fits approach where
 * it has multiple fixed-block allocators for different size classes.
 */
class MemoryPool {
public:
    /**
     * @brief Constructs a MemoryPool.
     * 
     * @param initial_pool_size The initial size of the memory pool in bytes.
     */
    explicit MemoryPool(size_t initial_pool_size);

    /**
     * @brief Destructor. Frees all allocated memory.
     */
    ~MemoryPool();

    /**
     * @brief Allocates a block of memory.
     * 
     * @param size The size of the block to allocate.
     * @return Pointer to the allocated block, or nullptr if allocation fails.
     */
    void* Allocate(size_t size);

    /**
     * @brief Deallocates a block of memory.
     * 
     * @param ptr Pointer to the block to deallocate.
     * @param size The size of the block to deallocate.
     */
    void Deallocate(void* ptr, size_t size);

    /**
     * @brief Gets the total size of the memory pool.
     * 
     * @return The total size in bytes.
     */
    size_t GetTotalSize() const { return total_size_; }

    /**
     * @brief Gets the used size of the memory pool.
     * 
     * @return The used size in bytes.
     */
    size_t GetUsedSize() const { return used_size_; }

    /**
     * @brief Gets the free size of the memory pool.
     * 
     * @return The free size in bytes.
     */
    size_t GetFreeSize() const { return total_size_ - used_size_; }

private:
    size_t total_size_;          ///< Total size of the memory pool
    size_t used_size_;           ///< Used size of the memory pool
    void* memory_pool_;          ///< Pointer to the allocated memory pool
    std::map<size_t, void*> free_blocks_; ///< Map of free blocks by size
};

} // namespace memory_pool

#endif // MEMORY_POOL_H
