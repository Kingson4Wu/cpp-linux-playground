# CMakeLists.txt for the Threaded Downloader project

# Minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(ThreadedDownloader VERSION 1.0)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---

# Find required packages
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

# --- Library ---

# Add the downloader library
add_library(downloader_lib
    src/downloader.cpp
    src/download_manager.cpp
    src/utils.cpp
)

# Specify include directories for the library
target_include_directories(downloader_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries to the downloader library
target_link_libraries(downloader_lib PUBLIC
    CURL::libcurl
    Threads::Threads
)

# --- Executable ---

# Add the main executable
add_executable(my_downloader
    src/main.cpp
)

# Link the downloader library to the executable
target_link_libraries(my_downloader PRIVATE
    downloader_lib
)

# Specify include directories for the executable
target_include_directories(my_downloader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Tests ---

# Add the tests subdirectory
add_subdirectory(tests)
