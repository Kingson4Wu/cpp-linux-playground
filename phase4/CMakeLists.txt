# CMakeLists.txt for phase4

# Add mini-redis subdirectory if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/mini-redis/CMakeLists.txt)
    add_subdirectory(mini-redis)
endif()

# Add mini-search subdirectory if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/mini-search/CMakeLists.txt)
    add_subdirectory(mini-search)
endif()

# Add crawler subdirectory if it exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/crawler/CMakeLists.txt)
    add_subdirectory(crawler)
endif()
