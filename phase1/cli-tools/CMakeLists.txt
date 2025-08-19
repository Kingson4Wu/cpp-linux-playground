# Create a static library for the core ls logic
add_library(ls_lib STATIC src/ls_lib.cpp)

# Make the include directory public so other targets can find the headers
target_include_directories(ls_lib PUBLIC include)

# Create the executable for the command-line tool
add_executable(my_ls src/ls_main.cpp)

# Link the executable against the library
target_link_libraries(my_ls PRIVATE ls_lib)
