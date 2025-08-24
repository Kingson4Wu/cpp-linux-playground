# CMakeLists.txt for mini-redis tests

# Find required packages
find_package(Threads REQUIRED)

# Create the test executable
add_executable(mini_redis_tests
    kv_store_test.cpp
    protocol_test.cpp
    redis_server_test.cpp
)

# Link the required libraries
target_link_libraries(mini_redis_tests 
    PRIVATE 
    mini_redis_lib 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

# Include the necessary directories
target_include_directories(mini_redis_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../phase4/mini-redis/include
)

# Add the test
add_test(NAME MiniRedisTests COMMAND mini_redis_tests)
