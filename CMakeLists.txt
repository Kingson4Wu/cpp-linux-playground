cmake_minimum_required(VERSION 3.20)
project(cpp-linux-playground)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add project subdirectories
add_subdirectory(phase1)

# --- Testing ---
# Enable testing
enable_testing()

# Include FetchContent for fetching dependencies
include(FetchContent)

# Declare Google Test dependency
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Make googletest available
FetchContent_MakeAvailable(googletest)

# Add the tests directory
add_subdirectory(tests)
