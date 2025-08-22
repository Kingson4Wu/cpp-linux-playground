# CMakeLists.txt for the Process Manager project

# Minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(ProcessManager VERSION 1.0)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library ---

# Add the process manager library
add_library(process_manager_lib
    src/process_reader.cpp
    src/system_info.cpp
)

# Specify include directories for the library
target_include_directories(process_manager_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Executable ---

# Add the main executable
add_executable(my_ps
    src/main.cpp
)

# Link the process manager library to the executable
target_link_libraries(my_ps PRIVATE
    process_manager_lib
)

# Specify include directories for the executable
target_include_directories(my_ps PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Tests ---

# Add the tests subdirectory
add_subdirectory(tests)
