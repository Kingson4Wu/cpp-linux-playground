# CMakeLists.txt for phase3 - Network Programming

# Add subdirectories for phase3 projects
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tcp-chat-room/CMakeLists.txt)
    add_subdirectory(tcp-chat-room)
endif()
