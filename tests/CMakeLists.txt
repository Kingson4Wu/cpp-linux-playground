# Add subdirectories for phase 1 tests
add_subdirectory(phase1)

# Add subdirectories for phase 2 tests
add_subdirectory(phase2/memory-pool)
add_subdirectory(phase2/process-manager)
add_subdirectory(phase2/threaded-downloader)

# Add subdirectories for phase 3 tests
add_subdirectory(phase3/tcp-chat-room)
add_subdirectory(phase3/http-server)
add_subdirectory(phase3/tcp-file-transfer)

# Add subdirectories for phase 4 tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/phase4/mini-redis/CMakeLists.txt)
    add_subdirectory(phase4/mini-redis)
endif()
