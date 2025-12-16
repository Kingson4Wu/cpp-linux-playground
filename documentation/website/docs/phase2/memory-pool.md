---
sidebar_position: 4
---

# Memory Pool Allocator Implementation

Develop a high-performance custom memory allocator that uses memory pools to reduce allocation overhead and improve cache locality. This project explores advanced memory management techniques and performance optimization.

## Project Structure

The memory pool allocator is organized as follows:

```
phase2/memory-pool/
├── CMakeLists.txt
├── include/
│   └── memory_pool.h  # Memory pool interface and classes
└── src/
    └── memory_pool.cpp  # Implementation of memory pool functionality
```

## Learning Objectives

During this project, you will:

- Implement custom memory allocation algorithms
- Understand memory fragmentation and allocation strategies
- Design lock-free or thread-safe memory pools
- Apply modern C++ features for memory management
- Measure and analyze memory allocation performance
- Implement memory debugging and tracking features

## Core Concepts

### Memory Pool Fundamentals

A memory pool pre-allocates a large block of memory and manages allocations within it:

- Reduces the number of system calls to `malloc`/`free`
- Improves cache locality for objects allocated together
- Can eliminate memory fragmentation in specific use cases
- Provides better performance for frequent allocations/deallocations of similar-sized objects

### Pool Types

Consider implementing different types of pools:

1. **Fixed-Size Pool**: For objects of identical size (most efficient)
2. **General Pool**: With sophisticated block management for variable sizes
3. **Thread-Specific Pool**: Per-thread pools to reduce synchronization overhead

## Implementation Architecture

### MemoryPool Interface

```cpp
class MemoryPool {
public:
    explicit MemoryPool(size_t pool_size);
    ~MemoryPool();
    
    void* allocate(size_t size, size_t alignment = alignof(std::max_align_t));
    void deallocate(void* ptr);
    
    size_t used_memory() const;
    size_t total_memory() const;
    double utilization() const;
    
    void reset();  // Free all allocations at once
    
private:
    char* pool_start_;
    char* pool_end_;
    size_t pool_size_;
    // Additional data structures for block management
};
```

### Fixed-Size Pool (Recommended First Implementation)

For fixed-size objects, implement a specialized pool:

```cpp
template<size_t BlockSize, size_t Alignment = alignof(std::max_align_t)>
class FixedSizePool {
public:
    explicit FixedSizePool(size_t num_blocks);
    ~FixedSizePool();
    
    void* allocate();
    void deallocate(void* ptr);
    
    size_t allocated_blocks() const;
    size_t total_blocks() const;
    
private:
    struct BlockHeader {
        BlockHeader* next_free;
    };
    
    char* memory_pool_;
    BlockHeader* free_list_;
    size_t total_blocks_;
    std::atomic<size_t> allocated_count_;
};
```

## Implementation Requirements

### Core Features

1. **Efficient Allocation**: Fast allocation and deallocation operations
2. **Memory Tracking**: Track used vs. free memory in the pool
3. **Alignment Support**: Properly align allocated blocks for different data types
4. **Reset Functionality**: Ability to free all allocations in O(1) time
5. **Memory Safety**: Prevent buffer overruns and double-deallocations

### Block Management Strategy

For general-purpose pools, implement a strategy to manage variable-sized allocations:

- Use a free list to track available blocks
- Implement first-fit, best-fit, or next-fit allocation algorithms
- Handle block splitting and coalescing to reduce fragmentation
- Consider using different pools for different size ranges (size classes)

### Sample Implementation Approach

```cpp
class MemoryPool {
private:
    struct Block {
        size_t size;
        bool is_free;
        Block* next;
        Block* prev;
    };
    
    char* pool_memory_;
    Block* free_blocks_;
    size_t pool_size_;
    std::mutex pool_mutex_;  // For thread safety
    
public:
    void* allocate(size_t size, size_t alignment);
    void deallocate(void* ptr);
    
private:
    Block* find_free_block(size_t size);
    void split_block(Block* block, size_t size);
    void merge_free_blocks(Block* block);
};
```

## Performance Considerations

- **Cache Locality**: Objects in the same pool are likely to be close in memory
- **Allocation Speed**: Aim for O(1) or O(log n) allocation time
- **Memory Overhead**: Minimize metadata overhead per allocation
- **Fragmentation**: Implement strategies to reduce fragmentation over time
- **Thread Safety**: Balance performance with safety for concurrent access

## Advanced Features (Optional)

Consider implementing additional capabilities:

1. **Thread-Caching**: Maintain per-thread pools to reduce synchronization
2. **Slab Allocation**: Specialized pools for specific object types
3. **Debugging Features**: Memory leak detection and bounds checking
4. **Statistics**: Detailed allocation statistics and profiling

## Testing Strategy

Test your memory pool with:

- Basic allocation/deallocation correctness
- Performance benchmarks against system allocator
- Stress tests with many allocations/deallocations
- Edge cases like allocating maximum pool size
- Thread safety tests for concurrent access
- Memory leak detection to ensure proper cleanup

## Usage Example

```cpp
#include "memory_pool.h"
#include <vector>

int main() {
    // Create a memory pool of 10MB
    MemoryPool pool(10 * 1024 * 1024);
    
    // Allocate objects using the pool
    std::vector<void*> allocations;
    for (int i = 0; i < 1000; ++i) {
        void* ptr = pool.allocate(64);  // 64-byte allocations
        allocations.push_back(ptr);
        
        // Check pool utilization
        std::cout << "Utilization: " << pool.utilization() * 100 << "%" << std::endl;
    }
    
    // Deallocate all objects
    for (void* ptr : allocations) {
        pool.deallocate(ptr);
    }
    
    // Or reset the entire pool at once
    // pool.reset();  // This would free all allocations at once
    
    return 0;
}
```

## Performance Benchmarking

Compare your memory pool against the system allocator:

```cpp
// Benchmark code example
auto start = std::chrono::high_resolution_clock::now();

// Test with system allocator
// ... perform allocations ...

auto mid = std::chrono::high_resolution_clock::now();

// Test with custom memory pool
// ... perform same allocations with pool ...

auto end = std::chrono::high_resolution_clock::now();
```

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# Run performance benchmarks to compare against system allocator
# The memory pool should show improved performance for frequent allocations
```

## Next Steps

After completing the memory pool, move on to [Phase 3: Network Programming](../phase3/intro.md) to explore network communication concepts.