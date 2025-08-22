#ifndef FIXED_BLOCK_ALLOCATOR_H
#define FIXED_BLOCK_ALLOCATOR_H

#include <cstddef>
#include <memory>

namespace memory_pool {

/**
 * @brief A fixed-size block allocator.
 * 
 * This allocator pre-allocates a large chunk of memory and divides it into
 * fixed-size blocks. It maintains a free list of available blocks for
 * efficient allocation and deallocation.
 */
class FixedBlockAllocator {
public:
    /**
     * @brief Constructs a FixedBlockAllocator.
     * 
     * @param block_size The size of each block in bytes.
     * @param num_blocks The number of blocks to allocate.
     */
    FixedBlockAllocator(size_t block_size, size_t num_blocks);

    /**
     * @brief Destructor. Frees the allocated memory pool.
     */
    ~FixedBlockAllocator();

    /**
     * @brief Allocates a block of memory.
     * 
     * @return Pointer to the allocated block, or nullptr if no blocks are available.
     */
    void* Allocate();

    /**
     * @brief Deallocates a block of memory.
     * 
     * @param ptr Pointer to the block to deallocate.
     */
    void Deallocate(void* ptr);

    /**
     * @brief Gets the size of each block.
     * 
     * @return The block size in bytes.
     */
    size_t GetBlockSize() const { return block_size_; }

    /**
     * @brief Gets the total number of blocks.
     * 
     * @return The total number of blocks.
     */
    size_t GetNumBlocks() const { return num_blocks_; }

    /**
     * @brief Gets the number of free blocks.
     * 
     * @return The number of free blocks.
     */
    size_t GetNumFreeBlocks() const { return num_free_blocks_; }

    /**
     * @brief Gets the number of used blocks.
     * 
     * @return The number of used blocks.
     */
    size_t GetNumUsedBlocks() const { return num_blocks_ - num_free_blocks_; }

private:
    size_t block_size_;          ///< Size of each block
    size_t num_blocks_;          ///< Total number of blocks
    size_t num_free_blocks_;     ///< Number of free blocks
    void* memory_pool_;          ///< Pointer to the allocated memory pool
    void** free_list_;           ///< Free list of available blocks
};

} // namespace memory_pool

#endif // FIXED_BLOCK_ALLOCATOR_H
