---
sidebar_position: 2
---

# Testing Standards

Comprehensive testing is essential for ensuring code quality, reliability, and maintainability in the Linux C++ Backend Development Playground project. This document outlines the testing standards and practices to follow.

## Testing Philosophy

- **Test-Driven Development (TDD)**: Write tests first when appropriate
- **Quality over Quantity**: Well-designed tests provide more value than numerous poorly-designed ones
- **Continuous Integration**: All tests must pass before code is merged
- **Performance Testing**: Include performance benchmarks for critical components
- **Memory Safety**: Verify proper memory management and absence of leaks

## Test Organization

### Directory Structure

Tests are organized by phase and project:

```
tests/
├── phase1/
│   ├── cli-tools/
│   ├── json-parser/
│   └── logger/
├── phase2/
│   ├── memory-pool/
│   ├── process-manager/
│   └── threaded-downloader/
├── phase3/
│   ├── tcp-chat-room/
│   ├── http-server/
│   └── tcp-file-transfer/
└── phase4/
    ├── crawler/
    ├── mini-redis/
    └── mini-search/
```

### Test File Naming

- Use `_test.cpp` suffix: `my_component_test.cpp`
- Match test file names to the source files being tested
- Group related tests in appropriate directories by phase

## Test Framework

### Google Test

- Use Google Test (gtest) as the primary testing framework
- Include gtest headers with `#include <gtest/gtest.h>`
- Follow Google Test best practices and patterns

### Test Structure

Use the AAA pattern (Arrange, Act, Assert):

```cpp
TEST(MyClassTest, ShouldBehaveCorrectly) {
    // Arrange - Set up test data
    MyClass obj;
    int input = 42;
    
    // Act - Execute the functionality being tested
    auto result = obj.process(input);
    
    // Assert - Verify the expected behavior
    EXPECT_EQ(result, 123);
}
```

## Test Categories

### Unit Tests

- Test individual functions and classes in isolation
- Mock dependencies when necessary
- Focus on specific functionality
- Fast execution time

```cpp
// Example unit test
TEST(BufferTest, ConstructorInitializesCorrectly) {
    Buffer buffer(1024);
    EXPECT_EQ(buffer.capacity(), 1024);
    EXPECT_EQ(buffer.size(), 0);
}
```

### Integration Tests

- Test how multiple components work together
- Verify interface compatibility
- Test data flow between components
- May use real dependencies instead of mocks

### Performance Tests

- Measure execution time and resource usage
- Identify performance bottlenecks
- Benchmark critical functions
- Track performance regressions

```cpp
// Example performance test
TEST(PerformanceTest, VectorVsListInsertion) {
    const size_t num_elements = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<int> vec;
    for (size_t i = 0; i < num_elements; ++i) {
        vec.push_back(i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_LT(duration.count(), 1000);  // Should complete in under 1 second
}
```

### Memory Safety Tests

- Use tools like AddressSanitizer and valgrind
- Test for memory leaks
- Verify proper cleanup in destructors
- Check for buffer overflows and invalid memory access

## Test Coverage

### Coverage Targets

- Aim for >90% code coverage for critical components
- Focus on testing business logic and error conditions
- Coverage is a guide, not the end goal - quality tests matter more

### Coverage Tools

- Use gcov and lcov for coverage analysis
- Generate HTML reports for easy review
- Include coverage in CI pipeline

## Test Writing Guidelines

### Naming Conventions

Use descriptive test names that explain what is being tested:

```cpp
// Good - Descriptive names
TEST(LoggerTest, SingleThreadedLoggingWorksCorrectly) {
    // test implementation
}

TEST(BufferTest, CopyConstructorCreatesIndependentCopy) {
    // test implementation
}

// Bad - Generic names
TEST(LoggerTest, Test1) { /* ... */ }
TEST(BufferTest, Test2) { /* ... */ }
```

### Test Isolation

- Each test should be independent of others
- Tests should not share mutable state
- Tests should be order-independent
- Set up and tear down test state within each test or fixture

### Parameterized Tests

Use parameterized tests for testing the same logic with different inputs:

```cpp
class MathTest : public ::testing::TestWithParam<std::tuple<int, int, int>> {};

TEST_P(MathTest, AdditionWorks) {
    auto [a, b, expected] = GetParam();
    EXPECT_EQ(a + b, expected);
}

INSTANTIATE_TEST_SUITE_P(
    BasicAddition,
    MathTest,
    ::testing::Values(
        std::make_tuple(1, 2, 3),
        std::make_tuple(5, 7, 12),
        std::make_tuple(-1, 1, 0)
    )
);
```

### Test Fixtures

Use fixtures for common setup/teardown logic:

```cpp
class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up before each test
        db_ = std::make_unique<Database>();
        test_data_ = create_test_data();
    }
    
    void TearDown() override {
        // Clean up after each test
        db_.reset();
        cleanup_test_data(test_data_);
    }
    
    std::unique_ptr<Database> db_;
    TestData test_data_;
};
```

## Mocking and Test Doubles

### When to Mock

- Mock external dependencies (files, network, databases)
- Mock complex objects that are expensive to create
- Mock when you need to control specific behavior for testing

### Google Mock

Use Google Mock for creating mocks:

```cpp
class MockNetworkClient {
public:
    MOCK_METHOD(bool, connect, (const std::string& host, int port));
    MOCK_METHOD(bool, send, (const std::string& data));
    MOCK_METHOD(std::string, receive, ());
};

TEST(NetworkServiceTest, ConnectFailureIsHandled) {
    MockNetworkClient mock_client;
    EXPECT_CALL(mock_client, connect("localhost", 8080))
        .WillOnce(Return(false));
    
    NetworkService service(&mock_client);
    EXPECT_FALSE(service.initialize());
}
```

## Common Test Patterns

### Boundary Value Testing

Test edge cases and boundary conditions:

```cpp
TEST(BufferTest, HandlesEmptyBuffer) {
    Buffer buffer(0);
    EXPECT_TRUE(buffer.empty());
    EXPECT_EQ(buffer.size(), 0);
}

TEST(BufferTest, HandlesFullBuffer) {
    Buffer buffer(10);
    for (int i = 0; i < 10; ++i) {
        buffer.push_back(i);
    }
    EXPECT_TRUE(buffer.full());
}
```

### Error Condition Testing

Test error handling and invalid input:

```cpp
TEST(ParserTest, ThrowsOnInvalidJson) {
    JsonParser parser;
    EXPECT_THROW(parser.parse("{ invalid json"), std::invalid_argument);
}
```

### State Transition Testing

For stateful objects, test state transitions:

```cpp
TEST(ConnectionTest, StateTransitionsAreCorrect) {
    Connection conn;
    EXPECT_EQ(conn.get_state(), ConnectionState::Disconnected);
    
    conn.connect();
    EXPECT_EQ(conn.get_state(), ConnectionState::Connected);
    
    conn.disconnect();
    EXPECT_EQ(conn.get_state(), ConnectionState::Disconnected);
}
```

## Test Execution

### Running Tests

Tests are run through the CI system and can also be executed manually:

```bash
# Run all tests
./scripts/docker-dev.sh test

# Build and run tests directly
cd build
make test  # or run ctest
```

### Test Selection

Use test filters to run specific tests:

```bash
# Run specific test suite
./test_executable --gtest_filter=MyTestSuite.*

# Run specific test
./test_executable --gtest_filter=MyTestSuite.SpecificTest

# Run tests matching pattern
./test_executable --gtest_filter=*Integration*
```

## Continuous Integration

### CI Requirements

- All tests must pass before merging
- Coverage should not decrease significantly
- Performance tests should not show significant regressions
- Memory safety tools should not detect issues

### Test Reporting

- Test results should be available in CI logs
- Coverage reports should be generated
- Performance benchmarks should be tracked over time

## Testing Anti-Patterns to Avoid

### Fragile Tests

- Avoid tests that break easily when code changes
- Don't test implementation details, test behavior
- Use appropriate abstraction levels in tests

### Slow Tests

- Keep unit tests fast (ideally under 1ms each)
- Use mocks for expensive operations
- Consider separate execution for slow integration tests

### Complex Setup

- Keep test setup simple and readable
- Use test fixtures for common setup
- Avoid overly complex test helpers

## Memory Safety Testing

### Tools for Memory Safety

- Use AddressSanitizer during development: `g++ -fsanitize=address ...`
- Run tests with valgrind for thorough memory checking
- Use static analysis tools as part of the CI pipeline

### Testing Memory Management

```cpp
TEST(MemoryPoolTest, HandlesMultipleAllocationsAndDeallocations) {
    MemoryPool pool(1024 * 1024);  // 1MB pool
    
    std::vector<void*> allocations;
    for (int i = 0; i < 100; ++i) {
        void* ptr = pool.allocate(64);
        allocations.push_back(ptr);
    }
    
    for (void* ptr : allocations) {
        pool.deallocate(ptr);
    }
    
    // At this point, no memory leaks should be detected by sanitizer
}
```

## Performance Testing

### Benchmarking Guidelines

- Measure realistic scenarios
- Run tests multiple times and average results
- Minimize external factors affecting performance
- Include performance tests in CI with appropriate thresholds

### Example Performance Test

```cpp
TEST(HashMapTest, PerformanceIsAcceptable) {
    const int num_operations = 100000;
    
    // Measure insertion performance
    HashMap<int, int> map;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < num_operations; ++i) {
        map.put(i, i * 2);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Ensure insertion doesn't take more than X milliseconds per operation
    EXPECT_LT(duration.count(), 1000);  // Total should be under 1 second for 100k ops
}
```

## Next Steps

Apply these testing standards to all your projects. Review the [Git Commit Standards](./git-commit.md) next to understand how to properly format and organize your commits for this project.