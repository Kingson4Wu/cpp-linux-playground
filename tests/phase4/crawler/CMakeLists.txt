# CMakeLists.txt for crawler tests

# Find required packages
find_package(Threads REQUIRED)

# Create the test executable
add_executable(crawler_tests
    url_parser_test.cpp
    http_client_test.cpp
    thread_pool_test.cpp
    crawler_test.cpp
    crawler_tests_main.cpp
)

# Link the required libraries
target_link_libraries(crawler_tests 
    PRIVATE 
    crawler_lib 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

# Include the necessary directories
target_include_directories(crawler_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../phase4/crawler/include
)

# Add the test
add_test(NAME CrawlerTests COMMAND crawler_tests)