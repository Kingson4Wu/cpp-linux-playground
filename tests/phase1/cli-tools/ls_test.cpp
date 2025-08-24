/*
 * ls_test.cpp - Unit tests for the ls command implementation
 *
 * How to run these tests:
 *
 * 1. Build the project:
 *    mkdir -p build && cd build && cmake .. && make
 *
 * 2. Run all tests:
 *    ctest
 *
 * 3. Run the test executable directly:
 *    ./tests/phase1/cli-tools/ls_test
 *
 * 4. Run a specific test case:
 *    ./tests/phase1/cli-tools/ls_test --gtest_filter=LsTest.ListsDirectoryContentsSorted
 *
 * Test cases covered:
 * - ListsDirectoryContentsSorted: Verifies directory contents are listed in alphabetical order
 * - ListsEmptyDirectory: Tests behavior with an empty directory
 * - ListsSingleFile: Tests listing a single file
 * - HandlesNonExistentPath: Tests error handling for non-existent paths
 */

#include "gtest/gtest.h"
#include "ls.h"
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

// Helper function to split string by newline
std::vector<std::string> split_lines(const std::string& str) {
    std::vector<std::string> lines;
    std::string line;
    std::istringstream stream(str);
    while (std::getline(stream, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return lines;
}

class LsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a unique temporary directory for testing
        test_dir = std::filesystem::temp_directory_path() / "ls_test_dir";
        std::filesystem::create_directory(test_dir);

        // Create some files and a subdirectory
        std::ofstream(test_dir / "file_z.txt").close();
        std::ofstream(test_dir / "file_a.txt").close();
        std::filesystem::create_directory(test_dir / "subdir_b");
    }

    void TearDown() override {
        // Clean up the temporary directory
        std::filesystem::remove_all(test_dir);
    }

    std::filesystem::path test_dir;
};

TEST_F(LsTest, ListsDirectoryContentsSorted) {
    std::stringstream ss;
    bool success = list_directory(test_dir, ss);

    auto lines = split_lines(ss.str());

    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "file_a.txt");
    EXPECT_EQ(lines[1], "file_z.txt");
    EXPECT_EQ(lines[2], "subdir_b");
    EXPECT_TRUE(success);
}

TEST_F(LsTest, ListsEmptyDirectory) {
    auto empty_dir = test_dir / "empty_subdir";
    std::filesystem::create_directory(empty_dir);

    std::stringstream ss;
    bool success = list_directory(empty_dir, ss);

    EXPECT_TRUE(ss.str().empty());
    EXPECT_TRUE(success);
}

TEST_F(LsTest, ListsSingleFile) {
    auto file_path = test_dir / "file_a.txt";
    std::stringstream ss;
    bool success = list_directory(file_path, ss);

    auto lines = split_lines(ss.str());
    ASSERT_EQ(lines.size(), 1);
    EXPECT_EQ(lines[0], "file_a.txt");
    EXPECT_TRUE(success);
}

TEST(LsSimpleTest, HandlesNonExistentPath) {
    std::filesystem::path non_existent_path = "non_existent_dir_12345";
    std::stringstream ss;
    bool success = list_directory(non_existent_path, ss);

    auto lines = split_lines(ss.str());
    ASSERT_EQ(lines.size(), 1);
    EXPECT_EQ(lines[0], "Error: Path does not exist.");
    EXPECT_FALSE(success);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
