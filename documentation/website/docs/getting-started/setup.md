---
sidebar_position: 1
---

# Setting Up Your Environment

Learn how to set up your development environment for the Linux C++ Backend Development Playground project.

## Prerequisites

Before starting, ensure you have the following tools installed on your system:

- **Git** (>= 2.0)
- **Docker** (>= 20.0) and **Docker Compose**
- **Node.js** (>= 18.0) - only needed for documentation development
- Basic command-line familiarity

## Docker Development Environment (Recommended)

The project uses Docker to provide a consistent development environment across platforms:

1. Clone the repository:
   ```bash
   git clone https://github.com/Kingson4Wu/cpp-linux-playground.git
   cd cpp-linux-playground
   ```

2. Build the development Docker image:
   ```bash
   ./scripts/docker-dev.sh build
   ```

3. Start the development container:
   ```bash
   ./scripts/docker-dev.sh run
   ```

4. Enter the development container:
   ```bash
   ./scripts/docker-dev.sh exec
   ```

Inside the container, you'll have access to all necessary tools including g++, CMake, Ninja, and debugging utilities.

## Alternative: Local Installation (Advanced)

If you prefer to work directly on your host system (Linux/macOS):

1. Install build essentials:
   ```bash
   # Ubuntu/Debian
   sudo apt update && sudo apt install build-essential
   
   # CentOS/RHEL
   sudo yum groupinstall "Development Tools"
   
   # macOS
   xcode-select --install
   ```

2. Install C++ compiler and CMake:
   ```bash
   # Ubuntu/Debian
   sudo apt install g++ cmake
   
   # CentOS/RHEL
   sudo yum install gcc gcc-c++ cmake
   
   # macOS
   brew install cmake
   ```

3. Install additional dependencies:
   ```bash
   # Ubuntu/Debian
   sudo apt install libboost-all-dev libssl-dev gdb lldb clang-format valgrind
   ```

## Verify Installation

After setting up your environment, verify everything works by:

1. Creating a build directory:
   ```bash
   mkdir build && cd build
   ```

2. Configuring the project:
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Debug
   ```

3. Building the project:
   ```bash
   make -j$(nproc)
   ```

If the build completes without errors, your environment is properly set up!

## Next Steps

Now that your environment is ready, continue to the [Project Overview](./project-overview.md) to understand the structure and progression of the learning materials.