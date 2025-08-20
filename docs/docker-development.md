# Docker Development Environment

This project includes a Docker development environment that provides a consistent Linux environment for building, testing, and debugging the C++ code.

## Prerequisites

- Docker
- Docker Compose

## Getting Started

1. Build the Docker image:
   ```bash
   ./scripts/docker-dev.sh build
   ```

2. Start the container in the background:
   ```bash
   ./scripts/docker-dev.sh run
   ```

3. Enter the container:
   ```bash
   ./scripts/docker-dev.sh exec
   ```

## Running Tests

To run all tests in the Docker container:
```bash
./scripts/docker-dev.sh test
```

## Debugging

To debug the ls_test executable with gdb:
```bash
./scripts/docker-dev.sh debug
```

## Stopping the Environment

To stop the container:
```bash
./scripts/docker-dev.sh stop
```

To clean up containers and volumes:
```bash
./scripts/docker-dev.sh clean
```

## Development Workflow

1. Make changes to the code on your host machine (files are mounted into the container)
2. Enter the container to build and test
3. Changes are immediately reflected in the container

Inside the container, you can use the standard build commands:
```bash
mkdir -p build && cd build
cmake ..
make
ctest
```
