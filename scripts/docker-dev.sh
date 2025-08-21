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
        shift
        docker-compose run --rm cpp-dev bash -c "find . -name CMakeCache.txt -delete && rm -rf build/* && mkdir -p build && cd build && cmake .. && make && cd .. && ./build/phase1/cli-tools/my_ls $*"
        ;;
    run-grep)
        echo "Building and running my_grep in Docker container..."
        shift
        docker-compose run --rm cpp-dev bash -c "find . -name CMakeCache.txt -delete && rm -rf build/* && mkdir -p build && cd build && cmake .. && make && cd .. && ./build/phase1/cli-tools/my_grep $*"
        ;;
    run-wc)
        echo "Building and running my_wc in Docker container..."
        shift
        docker-compose run --rm cpp-dev bash -c "find . -name CMakeCache.txt -delete && rm -rf build/* && mkdir -p build && cd build && cmake .. && make && cd .. && ./build/phase1/cli-tools/my_wc $*"
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
    coverage)
        echo "Generating code coverage report in Docker container..."
        docker-compose run --rm cpp-dev bash -c "
            # Create coverage build directory
            rm -rf build_coverage && mkdir -p build_coverage && cd build_coverage &&

            # Configure CMake with coverage enabled
            cmake -DENABLE_COVERAGE=ON .. &&

            # Build the project
            make -j\$(nproc) &&

            # Run tests
            ctest --output-on-failure &&

            # Create coverage directory
            mkdir -p coverage &&

            # Generate HTML coverage report
            gcovr -r .. --html-details coverage/index.html --exclude-directories='build.*' --exclude-unreachable-branches --exclude-throw-branches --exclude='.*test.*' &&

            # Generate XML coverage report
            gcovr -r .. --xml coverage/coverage.xml --exclude-directories='build.*' --exclude-unreachable-branches --exclude-throw-branches --exclude='.*test.*' &&

            # Generate console report
            echo '=== Coverage Report Generated ===' &&
            echo 'HTML report: /app/build_coverage/coverage/index.html' &&
            echo 'XML report: /app/build_coverage/coverage/coverage.xml' &&
            gcovr -r .. --exclude-directories='build.*' --exclude-unreachable-branches --exclude-throw-branches --exclude='.*test.*' &&
            echo 'Coverage report generated in build_coverage/coverage/'
        "
        ;;
    open-coverage)
        echo "Opening code coverage report in browser..."
        ./scripts/open_coverage.sh
        ;;
    *)
        echo "Usage: $0 {build|run|exec|test|coverage|open-coverage|debug|stop|clean}"
        echo "  build         - Build the Docker image"
        echo "  run           - Start the Docker container in background"
        echo "  exec          - Enter the running container"
        echo "  test          - Run all tests in Docker container"
        echo "  coverage      - Generate code coverage report"
        echo "  open-coverage - Open code coverage report in browser"
        echo "  debug         - Build and debug tests in Docker container"
        echo "  stop          - Stop the Docker container"
        echo "  clean         - Stop and remove containers and volumes"
        exit 1
        ;;
esac
