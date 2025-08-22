add_library(logger src/logger.cpp)

target_include_directories(logger PUBLIC include)

target_compile_features(logger PUBLIC cxx_std_20)

add_executable(logger_example src/logger_example.cpp)
target_link_libraries(logger_example logger)
