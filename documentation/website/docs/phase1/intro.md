---
sidebar_position: 1
---

# Phase 1: Syntax and Tool Familiarization

Phase 1 focuses on familiarizing you with modern C++20 syntax and building foundational command-line tools. This phase builds practical skills with the language while implementing simplified versions of common Unix utilities.

## Learning Objectives

By the end of Phase 1, you will:

- Master modern C++20 features including smart pointers, ranges, and coroutines
- Understand how to build robust command-line applications
- Implement file processing and text manipulation algorithms
- Apply proper error handling and logging techniques
- Use CMake for cross-platform builds
- Write comprehensive unit tests for your code

## Projects in This Phase

This phase consists of three main projects:

1. **CLI Tools Suite**: Simplified implementations of `ls`, `grep`, and `wc` commands
2. **JSON Parser**: A lightweight JSON parsing library
3. **Multi-threaded Logger**: A thread-safe logging system

Each project is designed to introduce you to different aspects of C++ programming while building practical, useful tools.

## Development Environment

All projects in Phase 1 should be developed and tested in the Docker environment for consistency:

```bash
# Enter development container
./scripts/docker-dev.sh exec

# Navigate to project root
cd /app

# Build Phase 1 projects
cd build
cmake ..
make -j4
```

## Testing Strategy

For each project in Phase 1, you should:

1. Write unit tests using Google Test framework
2. Test both normal and edge cases
3. Verify correct behavior with various inputs
4. Ensure proper error handling

Run tests with:
```bash
./scripts/docker-dev.sh test
```

## Next Steps

Begin with the [CLI Tools](./cli-tools.md) project to implement your first command-line utilities!