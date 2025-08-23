# CMakeLists.txt for tests

# Add executable for chat room tests
add_executable(chat_room_tests
    chat_room_test.cpp
)

# Link against the chat room library, Google Test libraries, and required system libraries
target_link_libraries(chat_room_tests
    chat_server_lib
    chat_client_lib
    GTest::gtest_main
    GTest::gmock
    pthread
)

# Include the project's include directory
target_include_directories(chat_room_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../phase3/tcp-chat-room/include
)

# Discover and add tests
include(GoogleTest)
gtest_discover_tests(chat_room_tests)
