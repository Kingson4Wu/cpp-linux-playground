# CMakeLists.txt for the TCP File Transfer project

# Minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(TcpFileTransfer VERSION 1.0)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library ---

# Create a library for the TCP file transfer server
add_library(tcp_file_transfer_lib
    src/tcp_file_server.cpp
    src/file_transfer_protocol.cpp
    src/thread_pool.cpp
)

# Specify include directories for the library
target_include_directories(tcp_file_transfer_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries for the library
find_package(Threads REQUIRED)
target_link_libraries(tcp_file_transfer_lib PUBLIC Threads::Threads)

# --- Executables ---

# Add the TCP file server executable
add_executable(tcp_file_server
    src/main_server.cpp
)

# Link the library to the executable
target_link_libraries(tcp_file_server PRIVATE tcp_file_transfer_lib)

# Specify include directories for the executable
target_include_directories(tcp_file_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add the TCP file client executable
add_executable(tcp_file_client
    src/main_client.cpp
    src/tcp_file_client.cpp
    src/file_transfer_protocol.cpp
)

# Specify include directories for the client executable
target_include_directories(tcp_file_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries for the client executable
target_link_libraries(tcp_file_client PRIVATE Threads::Threads)
