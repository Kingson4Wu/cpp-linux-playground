# Add the test executable
add_executable(ls_test ls_test.cpp)

# Link the test executable against our library and Google Test
target_link_libraries(ls_test PRIVATE ls_lib GTest::gtest_main)

# Add the test to CTest
include(GoogleTest)
gtest_discover_tests(ls_test)
