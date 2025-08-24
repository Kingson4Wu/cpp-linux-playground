# CMakeLists.txt for phase3 - Network Programming

# Add subdirectories for phase3 projects
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tcp-chat-room/CMakeLists.txt)
    add_subdirectory(tcp-chat-room)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/http-server/CMakeLists.txt)
    add_subdirectory(http-server)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tcp-file-transfer/CMakeLists.txt)
    add_subdirectory(tcp-file-transfer)
endif()
