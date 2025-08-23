# CMakeLists.txt for the Memory Pool Allocator project

# Minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(MemoryPool VERSION 1.0)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library ---

# Add the memory pool library
add_library(memory_pool_lib
    src/fixed_block_allocator.cpp
    src/memory_pool.cpp
)

# Specify include directories for the library
target_include_directories(memory_pool_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Executable ---

# Add the main executable for demonstration
add_executable(memory_pool_demo
    src/main.cpp
)

# Link the memory pool library to the executable
target_link_libraries(memory_pool_demo PRIVATE
    memory_pool_lib
)

# Specify include directories for the executable
target_include_directories(memory_pool_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Tests ---

# Add the tests subdirectory
# add_subdirectory(tests)  # Tests directory does not exist yet
