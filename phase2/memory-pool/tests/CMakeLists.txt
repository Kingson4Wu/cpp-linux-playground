# CMakeLists.txt for tests

# Add executable for memory pool tests
add_executable(memory_pool_tests
    memory_pool_test.cpp
)

# Link against the memory pool library, Google Test libraries, and required system libraries
target_link_libraries(memory_pool_tests
    memory_pool_lib
    GTest::gtest_main
    GTest::gmock
)

# Include the project's include directory
target_include_directories(memory_pool_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Discover and add tests
include(GoogleTest)
gtest_discover_tests(memory_pool_tests)
