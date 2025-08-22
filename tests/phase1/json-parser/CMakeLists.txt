# Create the executable for the JSON parser tests
add_executable(json_test json_test.cpp)

# Link the executable against the JSON library and Google Test
target_link_libraries(json_test PRIVATE json_lib gtest gtest_main)

# Register the test
add_test(NAME JsonTest COMMAND json_test)
