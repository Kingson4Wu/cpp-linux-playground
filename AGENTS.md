# Linux C++ Backend Development Hands-on Learning Project

This is a Linux C++ learning project for engineers with backend development experience, designed to master modern C++, system programming, network programming, and performance optimization through progressive hands-on projects.

## Project Overview

### Core Features
- **Language Standard**: C++20
- **Build Tool**: CMake (>= 3.20)
- **Development Environment**: Cross-platform (Mac/Windows/Linux) + Docker
- **Runtime Environment**: Linux Docker Container
- **Debugging Tools**: gdb/lldb + VSCode Remote-Containers

## Project Stages and Learning Path

### Phase 1: Syntax and Tool Familiarization
- Command-line tool suite (simplified versions of ls/grep/wc)
- JSON Parser
- Multi-threaded Logger

### Phase 2: System Programming
Goal: Master Linux system calls, process/thread management, and memory management
- Multi-threaded Downloader
- Process Management Tool (simplified ps/top)
- Memory Pool Allocator

### Phase 3: Network Programming
Goal: Be able to write stable, high-performance C++ network services
- TCP Multi-threaded Chat Room
- HTTP Static File Server
- TCP File Transfer Server

### Phase 4: Comprehensive Practice
Goal: Combine all skills to create usable backend services
- mini-Redis (KV Storage)
- mini-Search (Search Engine)
- High-concurrency Crawler

## Important Conventions and Standards

- **Compilation Standard**: `-std=c++20`
- **Build Directory**: `build/`
- **Source Directory**: Organized by phase (e.g., `phase1/`)
- **Test Files**: `*_test.cpp`
- **Header Files**: `include/` directory
- **CMake Configuration**: Independent CMakeLists.txt for each project
- **Documentation Standards**: 
  - Each executable sub-project's main source file header must contain comments explaining compilation, running, and usage methods
  - Each library implementation file must also contain usage instructions and running methods
  - Test files need to include instructions on how to run tests
- **Development Standards**:
  - All code comments and documentation must be in English
  - Do not commit code changes to Git unless explicitly instructed by the user
  - Before executing any task, output a specific implementation plan and wait for user confirmation before proceeding
- **Quality Assurance**:
  - Write unit tests to verify core functionality
  - Use code coverage tools to ensure test quality (target coverage > 90%)
  - Pay attention to memory safety, use valgrind/AddressSanitizer for detection
  - Unit tests must be written in the specified directory according to project standards (`tests/` directory organized by phase and project)
  - After each task is completed, all unit tests must be verified to pass successfully as a validation point
  - Verify unit tests are successful by running `./scripts/docker-dev.sh test` and checking for "100% tests passed" output
  - Projects must be able to successfully execute `cmake -S . -B build` and `cmake --build build` commands within the container
  - Note: Due to different paths inside and outside Docker containers, do not mix cmake execution inside and outside containers. Always execute cmake within the container, or clean the build directory when executing on the host machine.
  - You can verify builds within the container using the following commands:
    ```bash
    # Configure project
    docker-compose run --rm cpp-dev bash -c "cd /app && mkdir -p build && cd build && cmake .."

    # Build project
    docker-compose run --rm cpp-dev bash -c "cd /app && mkdir -p build && cd build && cmake .. && make -j4"
    ```

## Git Commit Standards

To maintain clear and consistent commit history, we follow professional Git commit standards:

### Commit Message Format
```
<type>(<scope>): <subject>

<body>

<footer>
```

### Commit Types
- `feat`: New feature
- `fix`: Bug fix
- `bug`: Bug fix (synonymous with `fix`)
- `docs`: Documentation update
- `style`: Code formatting adjustment (changes that don't affect code meaning, such as whitespace, formatting, etc.)
- `refactor`: Code refactoring (code changes that neither fix bugs nor add features)
- `perf`: Performance optimization
- `test`: Test-related
- `build`: Build system or external dependency changes
- `ci`: CI configuration files and script changes
- `chore`: Other changes that don't modify src or test files
- `revert`: Rollback previous commit

### Scope
Scope is optional and used to identify the module or component affected by the commit, such as:
- `cli-tools`
- `grep`
- `ls`
- `cmake`
- `docker`
- `tests`

### Commit Message Standards
1. Write commit messages in English
2. First line is a brief description (<72 characters)
3. Second line must be a genuine blank line (not using `\n` character)
4. Third line onwards is detailed description (optional)
5. Detailed description can include change reasons, impact scope, and other information

### Examples
```
feat(cli-tools): Implement basic functionality for grep command

- Add grep_lib.cpp library implementation file
- Add grep_main.cpp main program file
- Implement file search and pattern matching functionality

Resolves: #123
```

```
fix(grep): Fix parameter passing issue in Docker development script

- Fix issue where run-grep command cannot pass parameters to executable file
- Adjust working directory to allow grep command to correctly access files in project root directory
- Add shift command to properly handle parameters passed to script
```

## Key Technology Stack

- **Modern C++**: Smart pointers, lambda, range for, constexpr
- **Concurrent Programming**: std::thread, std::atomic, mutex, condition_variable
- **Network Programming**: socket, epoll, non-blocking IO
- **System Calls**: File IO, process management, memory mapping
- **Performance Optimization**: Memory pools, zero-copy, lock optimization

## Development Environment Setup

### Docker Development Container
```dockerfile
FROM ubuntu:24.04
RUN apt update && apt install -y \
    g++ cmake git build-essential ninja-build \
    libboost-all-dev libssl-dev gdb lldb clang-format valgrind
WORKDIR /app
```

### VSCode Configuration
- Required plugins: C/C++, CMake Tools, Remote-Containers
- Debugging configuration: Supports gdb/lldb debugging within container
- Code formatting: clang-format

## Notes and Best Practices

1. All projects must be compiled and run within Docker containers
2. Use CMake to manage builds, supporting Debug/Release modes
3. Write unit tests to verify core functionality
4. Use code coverage tools to ensure test quality (target coverage > 90%)
5. Pay attention to memory safety, use valgrind/AddressSanitizer for detection
6. Conduct performance testing and optimization after each project completion

## Code Standards

### File Organization
- Each sub-project contains independent `src` and `include` directories
- Library implementations are placed in the `src` directory
- Header files are placed in the `include` directory
- Test files are placed in the `tests` directory, organized by project phase

### Comment Standards
- Each source file header must contain file description and usage instructions
- Important functions need comments explaining functionality and parameters
- Complex logic needs comments explaining implementation approach

### CMake Standards
- Root CMakeLists.txt manages all sub-projects
- Each sub-project contains independent CMakeLists.txt
- Libraries are created using `add_library`
- Executables are created using `add_executable`
- Dependencies are linked using `target_link_libraries`

### Test Standards
- Use Google Test for unit testing
- Each test file needs to include instructions on how to run tests
- Test cases need to cover normal and exceptional situations
- Test passing standard: All test cases must pass 100%, and command return code must be 0. See `docs/test_pass_criteria.md` for details.

## Directory Structure Planning

```
.
├── CMakeLists.txt      # Root CMake configuration, manages all sub-projects
├── build/              # Build output directory (not committed to Git)
├── docs/               # Project documentation
├── docker/             # Docker-related configuration, such as Dockerfile
├── phase1/             # Phase 1: Syntax and tool familiarization
│   ├── CMakeLists.txt  # Aggregates phase1 projects
│   ├── cli-tools/
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   │   ├── grep.h
│   │   │   ├── ls.h
│   │   │   └── wc.h
│   │   └── src/
│   │       ├── grep_lib.cpp
│   │       ├── grep_main.cpp
│   │       ├── ls_lib.cpp
│   │       ├── ls_main.cpp
│   │       ├── wc_lib.cpp
│   │       └── wc_main.cpp
│   ├── json-parser/
│   └── logger/
├── phase2/             # Phase 2: System programming
│   ├── memory-pool/
│   ├── process-manager/
│   └── threaded-downloader/
├── phase3/             # Phase 3: Network programming
│   ├── tcp-chat-room/
│   ├── http-server/
│   └── tcp-file-transfer/
├── phase4/             # Phase 4: Comprehensive practice
│   ├── crawler/
│   ├── mini-redis/
│   └── mini-search/
├── scripts/            # Auxiliary scripts (build, test, run)
├── tests/              # Test files
│   ├── data/           # Test data
│   ├── phase1/
│   │   └── cli-tools/
│   │       ├── CMakeLists.txt
│   │       └── ls_test.cpp
│   ├── phase2/
│   │   ├── memory-pool/
│   │   ├── process-manager/
│   │   └── threaded-downloader/
│   ├── phase3/
│   │   ├── tcp-chat-room/
│   │   ├── http-server/
│   │   └── tcp-file-transfer/
│   └── phase4/
│       ├── crawler/
│       ├── mini-redis/
│       └── mini-search/
└── third_party/        # Third-party dependency libraries
```

## Learning Recommendations

1. **Progressive Learning**: Follow the phase order for learning, each phase's projects have specific learning objectives
2. **Hands-on Practice**: Not only read the code, but also implement and debug it yourself
3. **Focus on Performance**: While implementing functionality, pay attention to performance optimization and memory management
4. **Test-Driven**: Write unit tests for key functionality to ensure code quality
5. **Source Code Reading**: In later stages, read source code of excellent open-source projects like muduo and leveldb

## Current Progress

The project is currently in **Phase 4: Comprehensive Practice**, with completed **TCP Multi-threaded Chat Room**, **HTTP Static File Server**, **TCP File Transfer Server**, **mini-Redis**, **mini-Search**, and **High-concurrency Crawler**.

All projects have been written with unit tests according to requirements, and all tests have been verified to pass successfully. Test cases are written in the `tests/` directory under corresponding phase and project subdirectories according to project standards.

## Running Methods

### Using Docker Development Scripts
```bash
# Build Docker image
./scripts/docker-dev.sh build

# Start container
./scripts/docker-dev.sh run

# Enter container
./scripts/docker-dev.sh exec

# Build and run my_ls
./scripts/docker-dev.sh run-ls

# Build and run my_grep
./scripts/docker-dev.sh run-grep

# Build and run my_wc
./scripts/docker-dev.sh run-wc

# Run tests (using independent build-test directory)
./scripts/docker-dev.sh test

# Debug tests (using independent build directory)
./scripts/docker-dev.sh debug

# Stop container
./scripts/docker-dev.sh stop

# Clean up containers and volumes
./scripts/docker-dev.sh clean
```

**Note**: To improve development efficiency, different tasks use independent build directories:
*   `run-*` and `debug` commands use the `build/` directory.
*   `test` command uses the `build-test/` directory.
*   `coverage` command uses the `build_coverage/` directory.
This avoids unnecessary recompilation when switching between different tasks.

### Manual Compilation and Running
```bash
# From project root directory
cmake -S . -B build
cmake --build build -- -j
./build/phase1/cli-tools/my_ls [path]
```

### VSCode Development Container
1. Install "Dev Containers" and "CMake Tools" plugins
2. Open project in development container (VS Code will automatically attach to Docker)
3. CMake Tools automatically configures project (cmake -S . -B build)
4. Select target in bottom status bar (e.g., "my_ls")
5. Press F5 to build and debug program within container

### GitHub Actions Continuous Integration
The project is configured with GitHub Actions workflow that automatically runs unit tests when pushing to `main` branch or creating pull requests.

Workflow configuration file is located at `.github/workflows/ci.yml`, containing the following steps:
1. Set up build environment in Ubuntu 24.04
2. Install necessary dependencies (g++, cmake, build-essential, etc.)
3. Configure and build project
4. Run all unit tests

You can view build and test results through the "Actions" tab of the GitHub repository.

### Code Coverage
The project supports generating code coverage reports to ensure test quality:

```bash
# Generate coverage report using Docker
./scripts/docker-dev.sh coverage
```

Coverage reports will be generated in the `build_coverage/coverage/` directory:
- HTML format report: `build_coverage/coverage/index.html`
- XML format report: `build_coverage/coverage/coverage.xml`

The goal is to achieve over 90% code coverage to ensure core functionality is thoroughly tested.

Through this project's learning, developers can systematically master the core skills of Linux C++ backend development and lay a solid foundation for future high-performance backend development work.