# CMakeLists.txt for mini-search tests

# Find required packages
find_package(Threads REQUIRED)

# Create the test executable
add_executable(mini_search_tests
    document_test.cpp
    indexer_test.cpp
    query_processor_test.cpp
    search_engine_test.cpp
)

# Link the required libraries
target_link_libraries(mini_search_tests 
    PRIVATE 
    mini_search_lib 
    GTest::gtest 
    GTest::gtest_main
    Threads::Threads
)

# Include the necessary directories
target_include_directories(mini_search_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../phase4/mini-search/include
)

# Add the test
add_test(NAME MiniSearchTests COMMAND mini_search_tests)