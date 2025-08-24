# CMakeLists.txt for mini-search

# Find required packages
find_package(Threads REQUIRED)

# Create the library
add_library(mini_search_lib
    src/document.cpp
    src/indexer.cpp
    src/query_processor.cpp
    src/search_engine.cpp
)

# Link the required libraries
target_link_libraries(mini_search_lib 
    PRIVATE 
    Threads::Threads
)

# Include the necessary directories
target_include_directories(mini_search_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create the executable
add_executable(mini_search
    src/main.cpp
)

# Link the library to the executable
target_link_libraries(mini_search
    PRIVATE
    mini_search_lib
)

# Create the debug test executable
add_executable(debug_test
    src/debug_test.cpp
)

# Link the library to the debug test executable
target_link_libraries(debug_test
    PRIVATE
    mini_search_lib
)