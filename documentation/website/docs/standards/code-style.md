---
sidebar_position: 1
---

# Code Style and Formatting Standards

Maintaining consistent code style is essential for readability, maintainability, and collaboration. This document outlines the coding standards for the Linux C++ Backend Development Playground project.

## C++ Standard and Compiler

- **Language Standard**: C++20
- **Compiler**: g++ (GCC) or clang++ with `-std=c++20` flag
- **Build System**: CMake (minimum version 3.20)

## Naming Conventions

### Files and Directories

- Use lowercase with underscores: `my_component.h`, `my_component.cpp`
- Header files: `.h` extension
- Implementation files: `.cpp` extension
- Test files: `_test.cpp` suffix
- Directory names follow the same convention: `include/`, `src/`, `tests/`

### Variables and Functions

- **Variables**: Use snake_case for local variables and member variables
  ```cpp
  int student_count = 0;
  std::string user_name;
  ```

- **Functions**: Use snake_case for function names
  ```cpp
  void calculate_average();
  bool is_valid_input();
  ```

- **Constants**: Use SCREAMING_SNAKE_CASE for constants
  ```cpp
  const int MAX_CONNECTIONS = 100;
  constexpr double PI = 3.14159;
  ```

### Classes and Types

- **Class Names**: Use PascalCase for class names
  ```cpp
  class NetworkConnection;
  class MemoryPool;
  ```

- **Type Aliases**: Use PascalCase for type aliases
  ```cpp
  using BufferType = std::vector<char>;
  using Timestamp = std::chrono::system_clock::time_point;
  ```

- **Enum Classes**: Use PascalCase for enum classes and their values
  ```cpp
  enum class ConnectionState {
      Connected,
      Disconnected,
      Connecting
  };
  ```

## Code Formatting

### Indentation and Spacing

- Use 4 spaces for indentation (no tabs)
- Spaces around binary operators: `a + b`, not `a+b`
- Spaces after commas: `func(a, b, c)`, not `func(a,b,c)`
- No spaces inside parentheses: `func(arg)`, not `func( arg )`
- Spaces around control flow keywords: `if (condition)`, `while (condition)`

### Braces and Blocks

- Use K&R style (Egyptian) braces for functions:
  ```cpp
  void my_function() {
      // function body
  }
  ```

- Use K&R style for control structures:
  ```cpp
  if (condition) {
      // body
  } else {
      // else body
  }
  
  for (int i = 0; i < n; ++i) {
      // loop body
  }
  ```

### Line Length

- Maximum line length: 100 characters
- Break long lines after logical operators or before binary operators
- When breaking function signatures, align continuation properly

```cpp
// Good
auto result = some_function_with_a_very_long_name(
    first_parameter,
    second_parameter,
    third_parameter);

// Good - continuation aligned with opening parenthesis
auto long_variable_name = some_function(
    param1, param2,
    param3, param4);
```

## Class Design

### Member Variables

- Use trailing underscore for private member variables:
  ```cpp
  class MyClass {
  private:
      int count_;
      std::string name_;
  };
  ```

### Member Functions

- Follow RAII (Resource Acquisition Is Initialization) principles
- Use const-correctness for member functions that don't modify the object
- Prefer member initializer lists in constructors

```cpp
class Connection {
public:
    Connection(const std::string& host, int port) 
        : host_(host), port_(port), is_connected_(false) {}
    
    // const member function
    int get_port() const { return port_; }
    
    // modifying member function
    void connect();
    
private:
    std::string host_;
    int port_;
    bool is_connected_;
};
```

## Resource Management

### Smart Pointers

- Use `std::unique_ptr` for exclusive ownership
- Use `std::shared_ptr` for shared ownership with reference counting
- Use `std::weak_ptr` to break circular references
- Avoid raw pointers for ownership; use them only for non-owning references

```cpp
// Good: Exclusive ownership
std::unique_ptr<Connection> connection = std::make_unique<Connection>(host, port);

// Good: Shared ownership
std::shared_ptr<DataBuffer> buffer = std::make_shared<DataBuffer>();

// Good: Non-owning reference (use raw pointer)
void process_data(const DataBuffer* buffer);  // buffer is not owned by this function
```

### Memory Management

- Use RAII (Resource Acquisition Is Initialization) principles
- Prefer stack allocation over heap allocation when possible
- Use `std::make_unique` and `std::make_shared` instead of `new`
- Avoid explicit `delete` by using smart pointers

## Concurrency and Threading

### Thread Safety

- Use `std::mutex` and related primitives for synchronization
- Prefer `std::lock_guard` and `std::unique_lock` for RAII-based locking
- Use `std::atomic` for simple shared variables
- Document thread safety of classes and functions

```cpp
class ThreadSafeCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
    }
    
    int get_count() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }
    
private:
    mutable std::mutex mutex_;
    int count_{0};
};
```

### Best Practices

- Minimize lock contention by holding locks for the shortest time possible
- Use lock-free data structures when appropriate
- Design for thread safety from the beginning

## Error Handling

### Exceptions

- Use exceptions for error conditions that are exceptional
- Use error codes for expected conditions (like "file not found")
- Always document exception guarantees (nothrow, strong, basic)
- Prefer specific exception types over generic ones

```cpp
// Good: Specific exception
void validate_input(const std::string& input) {
    if (input.empty()) {
        throw std::invalid_argument("Input cannot be empty");
    }
}

// Good: Expected condition with error code
enum class ParseResult { Success, InvalidFormat, Overflow };
```

### Resource Cleanup

- Use RAII for automatic resource cleanup
- Implement proper destructors
- Use smart pointers to prevent memory leaks

## Comments and Documentation

### File Headers

Each source file should contain a header comment explaining the file's purpose:

```cpp
/*
 * file_name.cpp
 * 
 * Description of the file's purpose and functionality
 * 
 * Author: [Your name]
 * Date: [Date]
 */
```

### Function Documentation

Use Doxygen-style comments for public functions:

```cpp
/**
 * @brief Brief description of the function
 * 
 * @param param1 Description of first parameter
 * @param param2 Description of second parameter
 * @return Description of return value
 * @throws SpecificException Under what conditions
 */
int calculate_sum(int param1, int param2);
```

### Inline Comments

- Use inline comments sparingly, only to explain complex logic
- Comments should explain *why*, not *what* (the code shows what it does)
- Keep comments up to date with code changes

## Standard Library Usage

### Containers

- Use `std::vector` as the default sequence container
- Use `std::array` for fixed-size arrays
- Use `std::string` for string operations (not C-strings)
- Use `std::unordered_map` for hash-based lookups
- Use `std::map` for ordered key-value storage

### Algorithms

- Use standard algorithms instead of manual loops when appropriate
- Prefer `std::algorithm` functions over manual implementations

```cpp
// Good: Using standard algorithm
std::vector<int> numbers = {1, 2, 3, 4, 5};
auto min_val = std::min_element(numbers.begin(), numbers.end());

// Good: Range-based for loops when appropriate
for (const auto& item : container) {
    process(item);
}
```

## Modern C++ Features

### Prefer Modern C++17/C++20 Features

- Use structured bindings: `auto [key, value] = pair;`
- Use `std::optional` for values that might not exist
- Use `std::string_view` for string references
- Use `std::filesystem` for file operations
- Use class template argument deduction where available
- Use designated initializers (C++20): `Point p{.x = 1, .y = 2};`

## Build System (CMake)

### CMake Conventions

- Use lowercase for CMake commands
- Follow the project structure conventions
- Include proper target dependencies
- Use modern CMake practices

```cmake
# Good CMake example
add_executable(my_program
    src/main.cpp
    src/component.cpp
)

target_include_directories(my_program
    PRIVATE include/
)

target_link_libraries(my_program
    some_library
)
```

## Testing Standards

### Test Organization

- Place tests in `tests/` directory
- Name test files with `_test.cpp` suffix
- Use Google Test framework
- Test both positive and negative cases
- Write tests that are independent and deterministic

### Test Structure

Follow the AAA pattern (Arrange, Act, Assert):

```cpp
TEST(ConnectionTest, ConnectSucceeds) {
    // Arrange
    Connection conn("localhost", 8080);
    
    // Act
    bool result = conn.connect();
    
    // Assert
    EXPECT_TRUE(result);
}
```

## Git Commit Standards

Follow the project's Git commit standards:

- Use present tense in commit messages
- Start with a verb: "Add", "Fix", "Update", etc.
- First line should be a brief summary (under 72 characters)
- Follow with detailed description if needed
- Include issue references when applicable

```
feat(network): Add timeout support to Connection class

- Implement configurable connection timeout
- Add timeout parameter to connect() method
- Handle timeout errors gracefully
- Update unit tests for timeout scenarios

Fixes: #123
```

## Next Steps

Apply these standards consistently across all your code. Review the [Testing Standards](./testing.md) next to understand how to write effective tests for your C++ applications.