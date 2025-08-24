# CMakeLists.txt for mini-redis

# Find required packages
find_package(Threads REQUIRED)

# Create the library
add_library(mini_redis_lib
    src/kv_store.cpp
    src/protocol.cpp
    src/redis_server.cpp
    src/redis_client.cpp
)

# Link the required libraries
target_link_libraries(mini_redis_lib 
    PRIVATE 
    Threads::Threads
)

# Include the necessary directories
target_include_directories(mini_redis_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create the server executable
add_executable(redis_server
    src/main_server.cpp
)

# Link the library to the server executable
target_link_libraries(redis_server
    PRIVATE
    mini_redis_lib
)

# Create the client executable
add_executable(redis_client
    src/main_client.cpp
)

# Link the library to the client executable
target_link_libraries(redis_client
    PRIVATE
    mini_redis_lib
)
