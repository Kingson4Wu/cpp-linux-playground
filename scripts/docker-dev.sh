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
        docker-compose run --rm cpp-dev bash -c "mkdir -p build && cd build && (test ! -f CMakeCache.txt || grep -q \"CMAKE_BUILD_TYPE:STRING=Debug\" CMakeCache.txt || find . -name CMakeCache.txt -delete) && cmake -DCMAKE_BUILD_TYPE=Debug .. && make && cd .. && ./build/phase1/cli-tools/my_ls $*"
        ;;
    run-grep)
        echo "Building and running my_grep in Docker container..."
        shift
        docker-compose run --rm cpp-dev bash -c "mkdir -p build && cd build && (test ! -f CMakeCache.txt || grep -q \"CMAKE_BUILD_TYPE:STRING=Debug\" CMakeCache.txt || find . -name CMakeCache.txt -delete) && cmake -DCMAKE_BUILD_TYPE=Debug .. && make && cd .. && ./build/phase1/cli-tools/my_grep $*"
        ;;
    run-wc)
        echo "Building and running my_wc in Docker container..."
        shift
        docker-compose run --rm cpp-dev bash -c "mkdir -p build && cd build && (test ! -f CMakeCache.txt || grep -q \"CMAKE_BUILD_TYPE:STRING=Debug\" CMakeCache.txt || find . -name CMakeCache.txt -delete) && cmake -DCMAKE_BUILD_TYPE=Debug .. && make && cd .. && ./build/phase1/cli-tools/my_wc $*"
        ;;
    run-logger)
        echo "Building and running logger_example in Docker container..."
        shift
        docker-compose run --rm cpp-dev bash -c "mkdir -p build && cd build && (test ! -f CMakeCache.txt || grep -q \"CMAKE_BUILD_TYPE:STRING=Debug\" CMakeCache.txt || find . -name CMakeCache.txt -delete) && cmake -DCMAKE_BUILD_TYPE=Debug .. && make && cd .. && ./build/phase1/logger/logger_example $*"
        ;;
    test)
        echo "Running tests in Docker container..."
        echo "Test pass criteria: All tests must pass (100% pass rate) and exit code must be 0. See docs/test_pass_criteria.md for details."
        docker-compose run --rm cpp-dev bash -c "mkdir -p build-test && cd build-test && (test ! -f CMakeCache.txt || grep -q \"CMAKE_BUILD_TYPE:STRING=Release\" CMakeCache.txt || find . -name CMakeCache.txt -delete) && cmake -DCMAKE_BUILD_TYPE=Release .. && make && ctest"
        ;;
    debug)
        echo "Building and debugging ls_test in Docker container..."
        docker-compose run --rm cpp-dev bash -c "mkdir -p build && cd build && (test ! -f CMakeCache.txt || grep -q \"CMAKE_BUILD_TYPE:STRING=Debug\" CMakeCache.txt || find . -name CMakeCache.txt -delete) && cmake -DCMAKE_BUILD_TYPE=Debug .. && make && gdb ./tests/phase1/cli-tools/ls_test"
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
            mkdir -p build_coverage && cd build_coverage &&

            # Check and clean CMake cache if build type is not Coverage
            (test ! -f CMakeCache.txt || grep -q \"CMAKE_BUILD_TYPE:STRING=Coverage\" CMakeCache.txt || find . -name CMakeCache.txt -delete) &&

            # Configure CMake with coverage enabled and set build type
            cmake -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Coverage .. &&

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
