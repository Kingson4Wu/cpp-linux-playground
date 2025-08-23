# CMakeLists.txt for tests

# Add executable for HTTP server tests
add_executable(http_server_tests
    http_server_test.cpp
)

# Link against the HTTP server library, chat server library (for ThreadPool), Google Test libraries, and required system libraries
target_link_libraries(http_server_tests
    http_server_lib
    chat_server_lib
    GTest::gtest_main
    GTest::gmock
    pthread
)

# Include the project's include directory
target_include_directories(http_server_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../phase3/http-server/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../phase3/tcp-chat-room/include
)

# Discover and add tests
include(GoogleTest)
gtest_discover_tests(http_server_tests)
