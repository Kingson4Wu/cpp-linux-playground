# Add the ls test executable
add_executable(ls_test ls_test.cpp)

# Add the grep test executable
add_executable(grep_test grep_test.cpp)

# Link the test executables against our libraries and Google Test
target_link_libraries(ls_test PRIVATE ls_lib GTest::gtest_main)
target_link_libraries(grep_test PRIVATE grep_lib GTest::gtest_main)

# Add the tests to CTest
include(GoogleTest)
gtest_discover_tests(ls_test)
gtest_discover_tests(grep_test)
