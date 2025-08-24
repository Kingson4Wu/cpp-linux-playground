# CMakeLists.txt for phase2 - System Programming

# Add subdirectories for phase2 projects
# Only include threaded-downloader if CURL is available
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/threaded-downloader/CMakeLists.txt)
    add_subdirectory(threaded-downloader)
endif()

add_subdirectory(process-manager)
add_subdirectory(memory-pool)
