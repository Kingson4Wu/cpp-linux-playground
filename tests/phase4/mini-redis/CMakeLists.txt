# CMakeLists.txt for mini-redis tests

# Find required packages
find_package(Threads REQUIRED)

# Create the main test executable
add_executable(mini_redis_tests
    kv_store_test.cpp
    protocol_test.cpp
    redis_server_test.cpp
)

# Create a separate test executable for EXISTS command
add_executable(exists_command_test
    exists_command_test.cpp
)

# Link the required libraries
target_link_libraries(mini_redis_tests 
    PRIVATE 
    mini_redis_lib 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

target_link_libraries(exists_command_test
    PRIVATE 
    mini_redis_lib 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

# Include the necessary directories\ntarget_include_directories(mini_redis_tests PRIVATE\n    ${CMAKE_CURRENT_SOURCE_DIR}/../../phase4/mini-redis/include\n)\n\ntarget_include_directories(exists_command_test PRIVATE\n    ${CMAKE_CURRENT_SOURCE_DIR}/../../phase4/mini-redis/include\n)\n\n# Add the tests\nadd_test(NAME MiniRedisTests COMMAND mini_redis_tests)\nadd_test(NAME ExistsCommandTest COMMAND exists_command_test)
