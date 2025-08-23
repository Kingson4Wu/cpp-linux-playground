# CMakeLists.txt for tests

# Add executable for process manager tests
add_executable(process_manager_tests
    process_manager_test.cpp
)

# Link against the process manager library, Google Test libraries, and required system libraries
target_link_libraries(process_manager_tests
    process_manager_lib
    GTest::gtest_main
    GTest::gmock
)

# Include the project's include directory
target_include_directories(process_manager_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Discover and add tests
include(GoogleTest)
gtest_discover_tests(process_manager_tests)
