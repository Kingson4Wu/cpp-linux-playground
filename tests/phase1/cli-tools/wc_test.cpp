/*
 * wc_test.cpp - Unit tests for the wc command implementation
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
 *    ./tests/phase1/cli-tools/wc_test
 *
 * 4. Run a specific test case:
 *    ./tests/phase1/cli-tools/wc_test --gtest_filter=WcTest.CountsLinesWordsCharacters
 *
 * Test cases covered:
 * - CountsLinesWordsCharacters: Verifies wc counts lines, words, and characters correctly
 * - CountsLinesOnly: Tests wc with -l option
 * - CountsWordsOnly: Tests wc with -w option
 * - CountsCharactersOnly: Tests wc with -c option
 * - HandlesEmptyFile: Tests behavior with an empty file
 * - HandlesNonExistentFile: Tests error handling for non-existent files
 */

#include "gtest/gtest.h"
#include "wc.h"
#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>

class WcTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a unique temporary directory for testing
        test_dir = std::filesystem::temp_directory_path() / "wc_test_dir";
        std::filesystem::create_directory(test_dir);

        // Create a test file
        test_file = test_dir / "test.txt";
        std::ofstream file(test_file);
        file << "This is the first line\n";
        file << "This is the second line\n";
        file << "This is the third line with more words\n";
        file.close();
    }

    void TearDown() override {
        // Clean up the temporary directory
        std::filesystem::remove_all(test_dir);
    }

    std::filesystem::path test_dir;
    std::filesystem::path test_file;
};

TEST_F(WcTest, CountsLinesWordsCharacters) {
    WcResult result = wc_file(test_file, std::cerr);

    // Verify the counts are correct
    EXPECT_EQ(result.lines, 3);
    EXPECT_EQ(result.words, 18);
    EXPECT_EQ(result.characters, 86);
    EXPECT_TRUE(result.success);
}

TEST_F(WcTest, HandlesEmptyFile) {
    auto empty_file = test_dir / "empty.txt";
    std::ofstream file(empty_file);
    file.close();

    WcResult result = wc_file(empty_file, std::cerr);

    // Verify the counts are correct for an empty file
    EXPECT_EQ(result.lines, 0);
    EXPECT_EQ(result.words, 0);
    EXPECT_EQ(result.characters, 0);
    EXPECT_TRUE(result.success);
}

TEST(WcSimpleTest, HandlesNonExistentFile) {
    std::stringstream ss;
    WcResult result = wc_file("non_existent_file.txt", ss);

    // Verify that all counts are zero for a non-existent file
    EXPECT_EQ(result.lines, 0);
    EXPECT_EQ(result.words, 0);
    EXPECT_EQ(result.characters, 0);
    EXPECT_FALSE(result.success);

    // Verify that an error message was output
    EXPECT_NE(ss.str().find("Error: File does not exist"), std::string::npos);
}

TEST(WcTextTest, CountsLinesWordsCharacters) {
    std::string text = "First line\nSecond line with more words\nThird line\n";
    WcResult result = wc_text(text);

    // Verify the counts are correct
    EXPECT_EQ(result.lines, 3);
    EXPECT_EQ(result.words, 9);
    EXPECT_EQ(result.characters, 50);
}

TEST(WcTextTest, CountsLinesWordsCharactersNoTrailingNewline) {
    std::string text = "First line\nSecond line with more words\nThird line";
    WcResult result = wc_text(text);

    // Verify the counts are correct
    EXPECT_EQ(result.lines, 3);
    EXPECT_EQ(result.words, 9);
    EXPECT_EQ(result.characters, 49);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}