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
    # Include thread_pool.cpp if we need to copy it or link to the existing one
    # For now, we'll assume we can link to the existing chat_server_lib which contains ThreadPool
)

# Specify include directories for the library
target_include_directories(http_server_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # Include ThreadPool's header if needed
    ${CMAKE_CURRENT_SOURCE_DIR}/../tcp-chat-room/include
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

# Link to chat_server_lib to use ThreadPool
target_link_libraries(http_server PRIVATE chat_server_lib)
