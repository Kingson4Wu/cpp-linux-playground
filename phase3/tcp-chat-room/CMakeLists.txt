# CMakeLists.txt for the TCP Chat Room project

# Minimum CMake version
cmake_minimum_required(VERSION 3.20)

# Project name and version
project(TcpChatRoom VERSION 1.0)

# Require C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library ---

# Add the chat room library (if needed)
# For now, we'll put common code in the executables themselves
# If we need a library later, we can create one

# --- Executables ---

# Add the chat server executable
add_executable(chat_server
    src/server_main.cpp
    src/chat_server.cpp
    src/chat_session.cpp  # Add chat_session.cpp
    src/thread_pool.cpp
)

# Add the chat client executable
add_executable(chat_client
    src/client_main.cpp
    src/chat_client.cpp
)

# Specify include directories for the executables
target_include_directories(chat_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(chat_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
# For networking, we might need to link against system libraries
# On Unix-like systems, this is usually not necessary as socket functions are in libc
# But we might need to link against pthread for threading
find_package(Threads REQUIRED)
target_link_libraries(chat_server PRIVATE Threads::Threads)
target_link_libraries(chat_client PRIVATE Threads::Threads)

# --- Tests ---

# Add the tests subdirectory
# add_subdirectory(tests)
