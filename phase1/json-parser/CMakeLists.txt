# Create a static library for the JSON parser
add_library(json_lib STATIC src/json.cpp)

# Make the include directory public so other targets can find the headers
target_include_directories(json_lib PUBLIC include)

# Set the C++ standard to C++20
target_compile_features(json_lib PUBLIC cxx_std_20)

# Create an executable for the JSON parser example
add_executable(json_example src/json_example.cpp)

# Link the executable against the JSON library
target_link_libraries(json_example PRIVATE json_lib)
