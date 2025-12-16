---
sidebar_position: 2
---

# CLI Tools Implementation

Implement simplified versions of the classic Unix command-line tools: `ls`, `grep`, and `wc`. These projects will help you master basic C++ concepts and file system operations.

## Project Structure

The CLI tools are organized as follows:

```
phase1/cli-tools/
├── CMakeLists.txt
├── include/
│   ├── ls.h      # Header for ls functionality
│   ├── grep.h    # Header for grep functionality
│   └── wc.h      # Header for wc functionality
└── src/
    ├── ls_lib.cpp       # Implementation for ls functionality
    ├── ls_main.cpp      # Main entry point for ls command
    ├── grep_lib.cpp     # Implementation for grep functionality
    ├── grep_main.cpp    # Main entry point for grep command
    ├── wc_lib.cpp       # Implementation for wc functionality
    └── wc_main.cpp      # Main entry point for wc command
```

## ls Command Implementation

The `my_ls` command implements basic directory listing functionality.

### Features to Implement

- List files in current directory by default
- Accept directory path as argument
- Show detailed file information (size, permissions, timestamps)
- Support common flags like `-l` for long format

### Sample Implementation Approach

```cpp
// Example: Basic ls functionality
#include <filesystem>
#include <iostream>

void list_directory(const std::string& path) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::cout << entry.path().filename() << std::endl;
    }
}
```

## grep Command Implementation

The `my_grep` command implements pattern matching in files.

### Features to Implement

- Search for patterns in specified files
- Support for regular expressions
- Recursive directory search option
- Line number and file name reporting

### Sample Implementation Approach

```cpp
// Example: Basic grep functionality
#include <fstream>
#include <regex>
#include <string>

bool search_pattern(const std::string& filename, const std::string& pattern) {
    std::ifstream file(filename);
    std::string line;
    std::regex reg(pattern);
    
    int line_num = 1;
    while (std::getline(file, line)) {
        if (std::regex_search(line, reg)) {
            std::cout << filename << ":" << line_num << ":" << line << std::endl;
        }
        ++line_num;
    }
    return true;
}
```

## wc Command Implementation

The `my_wc` command implements word count functionality.

### Features to Implement

- Count words, lines, and characters in files
- Support multiple file inputs
- Display total counts when multiple files are provided
- Support options like `-w`, `-l`, `-c` for specific counts

### Sample Implementation Approach

```cpp
// Example: Basic wc functionality
#include <fstream>
#include <string>

struct Counts {
    int lines = 0;
    int words = 0;
    int chars = 0;
};

Counts count_file(const std::string& filename) {
    std::ifstream file(filename);
    Counts counts;
    std::string line;
    
    while (std::getline(file, line)) {
        counts.lines++;
        counts.chars += line.length() + 1;  // +1 for newline
        
        // Count words by splitting on whitespace
        // Implementation depends on your approach
    }
    
    return counts;
}
```

## Building and Running

To build and run the CLI tools:

```bash
# In the Docker container
cd /app

# Build the project
mkdir -p build && cd build
cmake ..
make -j4

# Run specific commands
./phase1/cli-tools/my_ls [directory_path]
./phase1/cli-tools/my_grep [pattern] [file_path]
./phase1/cli-tools/my_wc [file_path]
```

## Development Tips

- Use C++20 features like ranges when appropriate
- Implement proper error handling for file operations
- Pay attention to performance, especially for large files
- Follow the project's code standards and documentation guidelines

## Next Steps

After implementing the CLI tools, move on to the [JSON Parser](./json-parser.md) project to tackle structured data parsing.