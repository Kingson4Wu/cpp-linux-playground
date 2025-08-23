/**
 * @file memory_pool_test.cpp
 * @brief Unit tests for the memory pool allocator using Google Test.
 */

#include "fixed_block_allocator.h"
#include "memory_pool.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <thread>
#include <chrono>

// Test fixture for FixedBlockAllocator tests
class FixedBlockAllocatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code, if needed
    }

    void TearDown() override {
        // Cleanup code, if needed
    }
};

// Test to check if FixedBlockAllocator can be created
TEST_F(FixedBlockAllocatorTest, CanCreateAllocator) {
    const size_t block_size = 64;
    const size_t num_blocks = 10;

    EXPECT_NO_THROW({
        memory_pool::FixedBlockAllocator allocator(block_size, num_blocks);
    });
}

// Test to check if FixedBlockAllocator initializes correctly
TEST_F(FixedBlockAllocatorTest, InitializesCorrectly) {
    const size_t block_size = 64;
    const size_t num_blocks = 10;

    memory_pool::FixedBlockAllocator allocator(block_size, num_blocks);

    EXPECT_EQ(allocator.GetBlockSize(), block_size);
    EXPECT_EQ(allocator.GetNumBlocks(), num_blocks);
    EXPECT_EQ(allocator.GetNumFreeBlocks(), num_blocks);
    EXPECT_EQ(allocator.GetNumUsedBlocks(), 0);
}

// Test to check if FixedBlockAllocator can allocate blocks
TEST_F(FixedBlockAllocatorTest, CanAllocateBlocks) {
    const size_t block_size = 64;
    const size_t num_blocks = 10;

    memory_pool::FixedBlockAllocator allocator(block_size, num_blocks);

    // Allocate all blocks
    std::vector<void*> blocks;
    for (size_t i = 0; i < num_blocks; ++i) {
        void* block = allocator.Allocate();
        EXPECT_NE(block, nullptr);
        blocks.push_back(block);
    }

    // Check state
    EXPECT_EQ(allocator.GetNumFreeBlocks(), 0);
    EXPECT_EQ(allocator.GetNumUsedBlocks(), num_blocks);

    // Try to allocate one more block (should fail)
    void* extra_block = allocator.Allocate();
    EXPECT_EQ(extra_block, nullptr);
}

// Test to check if FixedBlockAllocator can deallocate blocks
TEST_F(FixedBlockAllocatorTest, CanDeallocateBlocks) {
    const size_t block_size = 64;
    const size_t num_blocks = 10;

    memory_pool::FixedBlockAllocator allocator(block_size, num_blocks);

    // Allocate all blocks
    std::vector<void*> blocks;
    for (size_t i = 0; i < num_blocks; ++i) {
        void* block = allocator.Allocate();
        EXPECT_NE(block, nullptr);
        blocks.push_back(block);
    }

    // Deallocate some blocks
    for (size_t i = 0; i < blocks.size(); i += 2) {
        allocator.Deallocate(blocks[i]);
    }

    // Check state
    EXPECT_EQ(allocator.GetNumFreeBlocks(), num_blocks / 2);
    EXPECT_EQ(allocator.GetNumUsedBlocks(), num_blocks - (num_blocks / 2));

    // Allocate again (should succeed for the deallocated blocks)
    for (size_t i = 0; i < num_blocks / 2; ++i) {
        void* block = allocator.Allocate();
        EXPECT_NE(block, nullptr);
    }

    // Check state
    EXPECT_EQ(allocator.GetNumFreeBlocks(), 0);
    EXPECT_EQ(allocator.GetNumUsedBlocks(), num_blocks);
}

// Test to check if FixedBlockAllocator handles invalid deallocations
TEST_F(FixedBlockAllocatorTest, HandlesInvalidDeallocations) {
    const size_t block_size = 64;
    const size_t num_blocks = 10;

    memory_pool::FixedBlockAllocator allocator(block_size, num_blocks);

    // Allocate a block
    void* block = allocator.Allocate();
    EXPECT_NE(block, nullptr);

    // Deallocate a null pointer (should not crash)
    EXPECT_NO_THROW(allocator.Deallocate(nullptr));

    // Deallocate a pointer not from this pool (should not crash)
    int some_int = 42;
    EXPECT_NO_THROW(allocator.Deallocate(&some_int));

    // Deallocate the same block twice (should not crash)
    EXPECT_NO_THROW(allocator.Deallocate(block));
    EXPECT_NO_THROW(allocator.Deallocate(block));
}

// Test fixture for MemoryPool tests
class MemoryPoolTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code, if needed
    }

    void TearDown() override {
        // Cleanup code, if needed
    }
};

// Test to check if MemoryPool can be created
TEST_F(MemoryPoolTest, CanCreatePool) {
    const size_t pool_size = 1024;

    EXPECT_NO_THROW({
        memory_pool::MemoryPool pool(pool_size);
    });
}

// Test to check if MemoryPool initializes correctly
TEST_F(MemoryPoolTest, InitializesCorrectly) {
    const size_t pool_size = 1024;

    memory_pool::MemoryPool pool(pool_size);

    EXPECT_EQ(pool.GetTotalSize(), pool_size);
    EXPECT_EQ(pool.GetUsedSize(), 0);
    EXPECT_EQ(pool.GetFreeSize(), pool_size);
}

// Test to check if MemoryPool can allocate blocks
TEST_F(MemoryPoolTest, CanAllocateBlocks) {
    const size_t pool_size = 1024;
    memory_pool::MemoryPool pool(pool_size);

    // Allocate some blocks
    void* block1 = pool.Allocate(100);
    EXPECT_NE(block1, nullptr);

    void* block2 = pool.Allocate(200);
    EXPECT_NE(block2, nullptr);

    // Check state
    EXPECT_EQ(pool.GetUsedSize(), 300);
    EXPECT_EQ(pool.GetFreeSize(), pool_size - 300);

    // Try to allocate a block that's too large (should fail)
    void* large_block = pool.Allocate(pool_size);
    EXPECT_EQ(large_block, nullptr);
}

// Test to check if MemoryPool can deallocate blocks
TEST_F(MemoryPoolTest, CanDeallocateBlocks) {
    const size_t pool_size = 1024;
    memory_pool::MemoryPool pool(pool_size);

    // Allocate some blocks
    void* block1 = pool.Allocate(100);
    EXPECT_NE(block1, nullptr);

    void* block2 = pool.Allocate(200);
    EXPECT_NE(block2, nullptr);

    // Deallocate a block
    pool.Deallocate(block1, 100);

    // Check state
    EXPECT_EQ(pool.GetUsedSize(), 200);
    EXPECT_EQ(pool.GetFreeSize(), pool_size - 200);

    // Deallocate another block
    pool.Deallocate(block2, 200);

    // Check state
    EXPECT_EQ(pool.GetUsedSize(), 0);
    EXPECT_EQ(pool.GetFreeSize(), pool_size);
}
