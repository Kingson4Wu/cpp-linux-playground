# CMakeLists.txt for tests

# Add executable for downloader tests
add_executable(downloader_tests
    downloader_test.cpp
)

# Link against the downloader library, Google Test libraries, and required system libraries
target_link_libraries(downloader_tests
    downloader_lib
    GTest::gtest_main
    GTest::gmock
)

# Include the project's include directory
target_include_directories(downloader_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Discover and add tests
include(GoogleTest)
gtest_discover_tests(downloader_tests)
