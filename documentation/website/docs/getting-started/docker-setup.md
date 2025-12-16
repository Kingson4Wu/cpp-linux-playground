---
sidebar_position: 3
---

# Docker Development Setup

Using Docker for development ensures a consistent environment across different platforms and simplifies dependency management for the Linux C++ Backend Development Playground.

## Why Docker?

The Docker development environment provides:

- Consistent development environment regardless of your host OS (macOS, Windows, Linux)
- Pre-installed development tools (g++, CMake, build-essential, Boost, OpenSSL, etc.)
- Isolated environment preventing conflicts with your system installations
- Easy switching between different project branches without dependency issues

## Docker Commands Reference

The project provides convenient scripts for Docker development:

```bash
# Build Docker image
./scripts/docker-dev.sh build

# Start container (detached mode)
./scripts/docker-dev.sh run

# Enter running container
./scripts/docker-dev.sh exec

# Stop container
./scripts/docker-dev.sh stop

# Clean up containers and volumes
./scripts/docker-dev.sh clean
```

## Building and Running Individual Phases

Once inside the container, you can build and run specific projects:

```bash
# Enter the container
./scripts/docker-dev.sh exec

# Navigate to project root
cd /app

# Build the entire project
mkdir -p build && cd build
cmake ..
make -j4

# Run specific executables (examples)
./build/phase1/cli-tools/my_ls [options]
./build/phase1/cli-tools/my_grep [options]
./build/phase1/cli-tools/my_wc [options]
```

## Development Workflow Inside Container

1. Enter the container:
   ```bash
   ./scripts/docker-dev.sh exec
   ```

2. Navigate to the project directory:
   ```bash
   cd /app
   ```

3. Create or modify source files in your host system (the `/app` directory is mounted)

4. Compile inside the container:
   ```bash
   cd build
   make -j4
   ```

5. Run executables:
   ```bash
   ./path/to/executable
   ```

## Debugging with GDB

The development container comes with debugging tools:

```bash
# Inside the container
cd /app/build

# Compile with debug symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j4

# Run with GDB
gdb ./path/to/executable
```

## Running Tests

Execute the project tests using the provided script:

```bash
# Run tests (uses build-test directory)
./scripts/docker-dev.sh test
```

## Visual Studio Code Integration

For the best development experience with Docker and VS Code:

1. Install the "Remote - Containers" extension
2. Open the project folder in VS Code
3. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on macOS) and select "Remote-Containers: Reopen in Container"
4. VS Code will build and connect to the development container
5. You can then develop, build, and debug from within the container environment

## Troubleshooting

### Common Issues

1. **Permission denied errors**: Make sure Docker is running and you have proper permissions to run Docker commands.

2. **Container won't start**: Try cleaning up with `./scripts/docker-dev.sh clean` and rebuild with `./scripts/docker-dev.sh build`.

3. **Files not syncing between host and container**: Files edited on the host should be available immediately in the container since `/app` is mounted as a volume.

## Next Steps

With your Docker environment configured, head to [Phase 1: Syntax and Tool Familiarization](../phase1/intro.md) to begin your C++ learning journey!