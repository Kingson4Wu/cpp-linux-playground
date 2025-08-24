# CMakeLists.txt for crawler

# Find required packages
find_package(Threads REQUIRED)

# Create the library
add_library(crawler_lib
    src/url_parser.cpp
    src/http_client.cpp
    src/crawler.cpp
    src/thread_pool.cpp
)

# Link the required libraries
target_link_libraries(crawler_lib 
    PRIVATE 
    Threads::Threads
)

# Include the necessary directories
target_include_directories(crawler_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create the executable
add_executable(crawler
    src/main.cpp
)

# Link the library to the executable
target_link_libraries(crawler
    PRIVATE
    crawler_lib
)