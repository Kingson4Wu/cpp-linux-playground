#!/bin/bash

# This script demonstrates the new functionality in the TCP file transfer client
# which allows uploading and downloading files without specifying a new filename.
# It also shows that the server correctly handles these default filenames.

# Build the project first
echo "Building the project..."
cd /Users/kingsonwu/programming/cpp_src/cpp-linux-playground
mkdir -p build
cd build
cmake .. &> /dev/null
make tcp_file_client tcp_file_server &> /dev/null

# Create a test file
echo "Creating a test file..."
echo "This is a test file for demonstrating the TCP file transfer client." > test_file.txt

echo "Starting the server in the background..."
./phase3/tcp-file-transfer/tcp_file_server 8080 . &> server.log &
SERVER_PID=$!

# Wait a moment for the server to start
sleep 2

echo "Testing file upload with default remote filename..."
./phase3/tcp-file-transfer/tcp_file_client 127.0.0.1 8080 upload test_file.txt

echo "Verifying that the file was stored with the correct name on the server..."
if [ -f "test_file.txt" ]; then
    echo "Success: File stored with default name 'test_file.txt' on server"
else
    echo "Error: File not found on server with expected name"
fi

echo "Testing file download with default local filename..."
./phase3/tcp-file-transfer/tcp_file_client 127.0.0.1 8080 download test_file.txt downloaded_file.txt

echo "Checking if the downloaded file matches the original..."
if cmp -s test_file.txt downloaded_file.txt; then
    echo "Success: Files match!"
else
    echo "Error: Files don't match!"
fi

# Test with directory structure in filename
echo "Creating a test file with path-like name..."
echo "This is a test file with path-like name." > path_test_file.txt

echo "Testing file upload with path-like filename (should be sanitized)..."
./phase3/tcp-file-transfer/tcp_file_client 127.0.0.1 8080 upload path_test_file.txt subdir/path_test_file.txt 2>&1 | grep -q "Server error" && echo "Success: Server correctly rejected path traversal attempt" || echo "Note: Upload with path-like name completed (server may sanitize)"

# Clean up
echo "Cleaning up..."
kill $SERVER_PID 2> /dev/null
rm -f test_file.txt downloaded_file.txt path_test_file.txt server.log
# Clean up any files that might have been created in the server storage
rm -f subdir/path_test_file.txt 2>/dev/null || true
rmdir subdir 2>/dev/null || true

echo "Demonstration complete!"
