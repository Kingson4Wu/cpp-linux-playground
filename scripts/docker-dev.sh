#!/bin/bash

# Script to manage the Docker development environment

case "$1" in
    build)
        echo "Building Docker image..."
        docker-compose build
        ;;
    run)
        echo "Starting Docker container..."
        docker-compose up -d
        ;;
    exec)
        echo "Entering Docker container..."
        docker-compose exec cpp-dev /bin/bash
        ;;
    run-ls)
        echo "Building and running my_ls in Docker container..."
        docker-compose run --rm cpp-dev bash -c "find . -name CMakeCache.txt -delete && rm -rf build/* && mkdir -p build && cd build && cmake .. && make && ./phase1/cli-tools/my_ls"
        ;;
    run-grep)
        echo "Building and running my_grep in Docker container..."
        docker-compose run --rm cpp-dev bash -c "find . -name CMakeCache.txt -delete && rm -rf build/* && mkdir -p build && cd build && cmake .. && make && ./phase1/cli-tools/my_grep"
        ;;
    test)
        echo "Running tests in Docker container..."
        docker-compose run --rm cpp-dev bash -c "find . -name CMakeCache.txt -delete && rm -rf build/* && mkdir -p build && cd build && cmake .. && make && ctest"
        ;;
    debug)
        echo "Building and debugging ls_test in Docker container..."
        docker-compose run --rm cpp-dev bash -c "find . -name CMakeCache.txt -delete && rm -rf build/* && mkdir -p build && cd build && cmake .. && make && gdb ./tests/phase1/cli-tools/ls_test"
        ;;
    stop)
        echo "Stopping Docker container..."
        docker-compose down
        ;;
    clean)
        echo "Cleaning Docker containers and volumes..."
        docker-compose down -v
        ;;
    *)
        echo "Usage: $0 {build|run|exec|run-ls|run-grep|test|debug|stop|clean}"
        echo "  build    - Build the Docker image"
        echo "  run      - Start the Docker container in background"
        echo "  exec     - Enter the running container"
        echo "  run-ls   - Build and run my_ls in Docker container"
        echo "  run-grep - Build and run my_grep in Docker container"
        echo "  test     - Run all tests in Docker container"
        echo "  debug    - Build and debug ls_test in Docker container"
        echo "  stop     - Stop the Docker container"
        echo "  clean    - Stop and remove containers and volumes"
        exit 1
        ;;
esac
