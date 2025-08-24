# CMakeLists.txt for TCP File Transfer tests

# Find required packages
find_package(Threads REQUIRED)

# Create the test executable
add_executable(tcp_file_transfer_unit_tests
    file_transfer_protocol_test.cpp
    tcp_file_server_test.cpp
)

# Link the required libraries
target_link_libraries(tcp_file_transfer_unit_tests 
    PRIVATE 
    tcp_file_transfer_lib 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

# Include the necessary directories
target_include_directories(tcp_file_transfer_unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../phase3/tcp-file-transfer/include
)

# Add the test
add_test(NAME TcpFileTransferUnitTests COMMAND tcp_file_transfer_unit_tests)
