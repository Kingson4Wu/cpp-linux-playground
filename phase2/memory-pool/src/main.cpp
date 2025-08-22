/**
 * @file main.cpp
 * @brief Main entry point for the memory pool allocator demonstration.
 *
 * This application demonstrates the usage of the memory pool allocator
 * implemented in this project. It shows how to use both the fixed-block
 * allocator and the general-purpose memory pool.
 *
 * How to Compile and Run with Docker:
 *   1. Build and run memory_pool_demo in Docker container:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase2/memory-pool/memory_pool_demo
 *
 *   2. Or, to build and run manually in Docker:
 *      ./scripts/docker-dev.sh exec
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *      ./phase2/memory-pool/memory_pool_demo
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase2/memory-pool/memory_pool_demo
 *
 * Usage:
 *   ./build/phase2/memory-pool/memory_pool_demo
 *
 * Examples:
 *   # Run the demonstration
 *   ./build/phase2/memory-pool/memory_pool_demo
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "memory_pool_demo") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "fixed_block_allocator.h"
#include "memory_pool.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

// Function to demonstrate FixedBlockAllocator
void DemonstrateFixedBlockAllocator() {
    std::cout << "=== Fixed Block Allocator Demonstration ===" << std::endl;

    const size_t block_size = 64;
    const size_t num_blocks = 10;

    try {
        memory_pool::FixedBlockAllocator allocator(block_size, num_blocks);

        std::cout << "Allocator created with block size " << allocator.GetBlockSize() 
                  << " and " << allocator.GetNumBlocks() << " blocks." << std::endl;
        std::cout << "Free blocks: " << allocator.GetNumFreeBlocks() << std::endl;
        std::cout << "Used blocks: " << allocator.GetNumUsedBlocks() << std::endl;

        // Allocate some blocks
        std::vector<void*> blocks;
        for (size_t i = 0; i < num_blocks; ++i) {
            void* block = allocator.Allocate();
            if (block) {
                blocks.push_back(block);
                std::cout << "Allocated block " << i << " at " << block << std::endl;
            } else {
                std::cout << "Failed to allocate block " << i << std::endl;
                break;
            }
        }

        std::cout << "Free blocks: " << allocator.GetNumFreeBlocks() << std::endl;
        std::cout << "Used blocks: " << allocator.GetNumUsedBlocks() << std::endl;

        // Try to allocate one more block (should fail)
        void* extra_block = allocator.Allocate();
        if (!extra_block) {
            std::cout << "As expected, failed to allocate an extra block." << std::endl;
        }

        // Deallocate some blocks
        for (size_t i = 0; i < blocks.size(); i += 2) {
            allocator.Deallocate(blocks[i]);
            std::cout << "Deallocated block at " << blocks[i] << std::endl;
        }

        std::cout << "Free blocks: " << allocator.GetNumFreeBlocks() << std::endl;
        std::cout << "Used blocks: " << allocator.GetNumUsedBlocks() << std::endl;

        // Allocate again (should succeed for the deallocated blocks)
        for (size_t i = 0; i < blocks.size() / 2; ++i) {
            void* block = allocator.Allocate();
            if (block) {
                std::cout << "Re-allocated block at " << block << std::endl;
            } else {
                std::cout << "Failed to re-allocate block " << i << std::endl;
            }
        }

        std::cout << "Free blocks: " << allocator.GetNumFreeBlocks() << std::endl;
        std::cout << "Used blocks: " << allocator.GetNumUsedBlocks() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << std::endl;
}

// Function to demonstrate MemoryPool
void DemonstrateMemoryPool() {
    std::cout << "=== General Memory Pool Demonstration ===" << std::endl;

    const size_t pool_size = 1024;

    try {
        memory_pool::MemoryPool pool(pool_size);

        std::cout << "Pool created with size " << pool.GetTotalSize() << " bytes." << std::endl;
        std::cout << "Used size: " << pool.GetUsedSize() << " bytes." << std::endl;
        std::cout << "Free size: " << pool.GetFreeSize() << " bytes." << std::endl;

        // Allocate some blocks
        std::vector<std::pair<void*, size_t>> blocks;
        size_t sizes[] = {100, 200, 50, 300};

        for (size_t size : sizes) {
            void* block = pool.Allocate(size);
            if (block) {
                blocks.push_back({block, size});
                std::cout << "Allocated block of size " << size << " at " << block << std::endl;
            } else {
                std::cout << "Failed to allocate block of size " << size << std::endl;
            }
        }

        std::cout << "Used size: " << pool.GetUsedSize() << " bytes." << std::endl;
        std::cout << "Free size: " << pool.GetFreeSize() << " bytes." << std::endl;

        // Deallocate some blocks
        for (size_t i = 0; i < blocks.size(); i += 2) {
            pool.Deallocate(blocks[i].first, blocks[i].second);
            std::cout << "Deallocated block of size " << blocks[i].second 
                      << " at " << blocks[i].first << std::endl;
        }

        std::cout << "Used size: " << pool.GetUsedSize() << " bytes." << std::endl;
        std::cout << "Free size: " << pool.GetFreeSize() << " bytes." << std::endl;

        // Allocate again
        void* new_block = pool.Allocate(150);
        if (new_block) {
            std::cout << "Allocated new block of size 150 at " << new_block << std::endl;
        } else {
            std::cout << "Failed to allocate new block of size 150." << std::endl;
        }

        std::cout << "Used size: " << pool.GetUsedSize() << " bytes." << std::endl;
        std::cout << "Free size: " << pool.GetFreeSize() << " bytes." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << std::endl;
}

// Function to compare performance with std::malloc
void PerformanceComparison() {
    std::cout << "=== Performance Comparison (std::malloc vs MemoryPool) ===" << std::endl;

    const size_t num_allocations = 10000;
    const size_t block_size = 64;

    // Test std::malloc
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<void*> malloc_blocks;
    for (size_t i = 0; i < num_allocations; ++i) {
        void* block = std::malloc(block_size);
        if (block) {
            malloc_blocks.push_back(block);
        }
    }

    for (void* block : malloc_blocks) {
        std::free(block);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto malloc_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // Test FixedBlockAllocator
    start = std::chrono::high_resolution_clock::now();
    memory_pool::FixedBlockAllocator allocator(block_size, num_allocations);
    std::vector<void*> pool_blocks;
    for (size_t i = 0; i < num_allocations; ++i) {
        void* block = allocator.Allocate();
        if (block) {
            pool_blocks.push_back(block);
        }
    }

    for (void* block : pool_blocks) {
        allocator.Deallocate(block);
    }
    end = std::chrono::high_resolution_clock::now();
    auto pool_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "std::malloc time for " << num_allocations << " allocations/deallocations: " 
              << malloc_duration.count() << " microseconds" << std::endl;
    std::cout << "FixedBlockAllocator time for " << num_allocations << " allocations/deallocations: " 
              << pool_duration.count() << " microseconds" << std::endl;

    std::cout << std::endl;
}

int main() {
    try {
        DemonstrateFixedBlockAllocator();
        DemonstrateMemoryPool();
        PerformanceComparison();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
