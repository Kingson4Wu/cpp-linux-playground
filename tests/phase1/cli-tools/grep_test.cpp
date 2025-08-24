/*
 * grep_test.cpp - Unit tests for the grep command implementation
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
 *    ./tests/phase1/cli-tools/grep_test
 *
 * 4. Run a specific test case:
 *    ./tests/phase1/cli-tools/grep_test --gtest_filter=GrepTest.MatchesPatternInText
 *
 * Test cases covered:
 * - MatchesPatternInText: Verifies grep finds lines containing the pattern
 * - NoMatchInText: Tests behavior when pattern is not found
 * - MatchesWithLineNumbers: Tests grep with line number display
 * - HandlesNonExistentFile: Tests error handling for non-existent files
 */

#include "gtest/gtest.h"
#include "grep.h"
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

class GrepTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a unique temporary directory for testing
        test_dir = std::filesystem::temp_directory_path() / "grep_test_dir";
        std::filesystem::create_directory(test_dir);

        // Create a test file
        test_file = test_dir / "test.txt";
        std::ofstream file(test_file);
        file << "This is the first line\n";
        file << "This line contains the pattern\n";
        file << "Another line without it\n";
        file << "Pattern appears here too\n";
        file << "Final line\n";
        file.close();
    }

    void TearDown() override {
        // Clean up the temporary directory
        std::filesystem::remove_all(test_dir);
    }

    std::filesystem::path test_dir;
    std::filesystem::path test_file;
};

TEST_F(GrepTest, MatchesPatternInText) {
    // Verify file content
    std::ifstream verify_file(test_file);
    std::string line;
    int line_num = 0;
    std::cout << "Verifying file content:" << std::endl;
    while (std::getline(verify_file, line)) {
        line_num++;
        std::cout << line_num << ": " << line << std::endl;
    }
    verify_file.close();

    std::stringstream ss;
    bool success = grep_file("pattern", test_file, ss, false);

    // First check if function executed successfully
    EXPECT_TRUE(success);

    std::string result = ss.str();
    std::cout << "Actual grep output: " << result << std::endl;
    EXPECT_NE(result.find("This line contains the pattern"), std::string::npos);
    // Note: Since we're searching for "pattern" (lowercase), it won't match "Pattern" (uppercase)
    EXPECT_EQ(result.find("Pattern appears here too"), std::string::npos);
    EXPECT_EQ(result.find("This is the first line"), std::string::npos);
}

TEST_F(GrepTest, NoMatchInText) {
    std::stringstream ss;
    bool success = grep_file("nonexistent", test_file, ss, false);
    
    // Check if function executed successfully
    EXPECT_TRUE(success);

    std::string result = ss.str();
    EXPECT_TRUE(result.empty());
}

TEST_F(GrepTest, MatchesWithLineNumbers) {
    // Verify file content
    std::ifstream verify_file(test_file);
    std::string line;
    int line_num = 0;
    std::cout << "Verifying file content:" << std::endl;
    while (std::getline(verify_file, line)) {
        line_num++;
        std::cout << line_num << ": " << line << std::endl;
    }
    verify_file.close();

    std::stringstream ss;
    bool success = grep_file("pattern", test_file, ss, true);

    // First check if function executed successfully
    EXPECT_TRUE(success);

    std::string result = ss.str();
    std::cout << "Actual output with line numbers: " << result << std::endl;
    EXPECT_NE(result.find("2:This line contains the pattern"), std::string::npos);
    // Note: Since we're searching for "pattern" (lowercase), it won't match "Pattern" (uppercase)
    EXPECT_EQ(result.find("4:Pattern appears here too"), std::string::npos);
}

TEST(GrepSimpleTest, HandlesNonExistentFile) {
    std::stringstream ss;
    bool result = grep_file("pattern", "non_existent_file.txt", ss, false);

    EXPECT_FALSE(result);
    EXPECT_NE(ss.str().find("Error: File does not exist"), std::string::npos);
}

TEST(GrepTextTest, MatchesPatternInText) {
    std::string text = "First line\nSecond line with pattern\nThird line\npattern in fourth line\nFifth line";
    std::stringstream ss;
    bool success = grep_text("pattern", text, ss, false);
    
    // Check if function executed successfully
    EXPECT_TRUE(success);

    std::string result = ss.str();
    EXPECT_NE(result.find("Second line with pattern"), std::string::npos);
    EXPECT_NE(result.find("pattern in fourth line"), std::string::npos);
}

TEST(GrepTextTest, MatchesPatternInTextWithLineNumbers) {
    std::string text = "First line\nSecond line with pattern\nThird line\npattern in fourth line\nFifth line";
    std::stringstream ss;
    bool success = grep_text("pattern", text, ss, true);
    
    // Check if function executed successfully
    EXPECT_TRUE(success);

    std::string result = ss.str();
    EXPECT_NE(result.find("2:Second line with pattern"), std::string::npos);
    EXPECT_NE(result.find("4:pattern in fourth line"), std::string::npos);
}

TEST(GrepTextTest, HandlesEmptyText) {
    std::string text = "";
    std::stringstream ss;
    bool success = grep_text("pattern", text, ss, false);
    
    // Check if function executed successfully
    EXPECT_TRUE(success);
    
    // Result should be empty since there's no text to search
    std::string result = ss.str();
    EXPECT_TRUE(result.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}