# TCP File Transfer Server Tests

This directory contains unit tests for the TCP File Transfer Server.

## How to Run Tests

To run the tests, you need to build the project with testing enabled:

```bash
# From the project root directory
cmake -S . -B build
cmake --build build -- -j

# Run the tests
cd build
ctest
```

Or you can run the test executable directly:

```bash
# From the build directory
./phase3/tcp-file-transfer/tcp_file_transfer_tests
```

## Test Coverage

The tests cover the following components:

1. **File Transfer Protocol**:
   - Serialization and deserialization of messages
   - Handling of various message types (upload request, download request, etc.)
   - Error handling for invalid messages

2. **TCP File Server**:
   - File path resolution and security checks
   - Handling of upload and download requests
   - Server start and stop functionality

## Writing New Tests

To add new tests, create a new test file in this directory and add it to the `tcp_file_transfer_tests` executable in the CMakeLists.txt file.
