# Create a static library for the core ls logic
add_library(ls_lib STATIC src/ls_lib.cpp)

# Create a static library for the core grep logic
add_library(grep_lib STATIC src/grep_lib.cpp)

# Make the include directory public so other targets can find the headers
target_include_directories(ls_lib PUBLIC include)
target_include_directories(grep_lib PUBLIC include)

# Create the executable for the ls command-line tool
add_executable(my_ls src/ls_main.cpp)

# Create the executable for the grep command-line tool
add_executable(my_grep src/grep_main.cpp)

# Link the executables against their respective libraries
target_link_libraries(my_ls PRIVATE ls_lib)
target_link_libraries(my_grep PRIVATE grep_lib)
