# CMakeLists.txt for the TCP Chat Room project

# Minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(TcpChatRoom VERSION 1.0)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library ---

# Create a library for the chat server
add_library(chat_server_lib
    src/chat_server.cpp
    src/chat_session.cpp
    src/thread_pool.cpp
)

# Create a library for the chat client
add_library(chat_client_lib
    src/chat_client.cpp
)

# Specify include directories for the libraries
target_include_directories(chat_server_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(chat_client_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries for the libraries
find_package(Threads REQUIRED)
target_link_libraries(chat_server_lib PUBLIC Threads::Threads)
target_link_libraries(chat_client_lib PUBLIC Threads::Threads)

# --- Executables ---

# Add the chat server executable
add_executable(chat_server
    src/server_main.cpp
)

# Add the chat client executable
add_executable(chat_client
    src/client_main.cpp
)

# Link the libraries to the executables
target_link_libraries(chat_server PRIVATE chat_server_lib)
target_link_libraries(chat_client PRIVATE chat_client_lib)

# Specify include directories for the executables
target_include_directories(chat_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(chat_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
