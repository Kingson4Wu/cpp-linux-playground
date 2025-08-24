# CMakeLists.txt for the HTTP Server project

# Minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(HttpServer VERSION 1.0)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library ---

# Create a library for the HTTP server
add_library(http_server_lib
    src/http_request.cpp
    src/http_response.cpp
    src/http_connection_handler.cpp
    src/thread_pool.cpp
)

# Specify include directories for the library
target_include_directories(http_server_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries for the library
find_package(Threads REQUIRED)
target_link_libraries(http_server_lib PUBLIC Threads::Threads)

# --- Executables ---

# Add the HTTP server executable
add_executable(http_server
    src/main.cpp
)

# Link the library to the executable
target_link_libraries(http_server PRIVATE http_server_lib)

# Specify include directories for the executable
target_include_directories(http_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
