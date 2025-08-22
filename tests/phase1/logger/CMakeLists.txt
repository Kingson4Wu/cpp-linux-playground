add_executable(logger_test logger_test.cpp)

target_link_libraries(logger_test gtest_main logger)

include(GoogleTest)
gtest_discover_tests(logger_test)
