/**
 * @file process_manager_test.cpp
 * @brief Unit tests for the process manager library using Google Test.
 */

#include "process_reader.h"
#include "system_info.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unistd.h>
#include <cstdlib>

// Test fixture for ProcessManager tests
class ProcessManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code, if needed
    }

    void TearDown() override {
        // Cleanup code, if needed
    }
};

// Test to check if GetProcessList returns a non-empty list
TEST_F(ProcessManagerTest, GetProcessListReturnsProcesses) {
    std::vector<int> pids = process_manager::GetProcessList();
    // We expect at least one process to be running (this test process)
    EXPECT_FALSE(pids.empty());

    // Check that all PIDs are positive
    for (int pid : pids) {
        EXPECT_GT(pid, 0);
    }
}

// Test to check if ReadProcessInfo can read information for the current process
TEST_F(ProcessManagerTest, ReadProcessInfoForCurrentProcess) {
    int current_pid = getpid();
    process_manager::ProcessInfo info = process_manager::ReadProcessInfo(current_pid);

    EXPECT_EQ(info.pid, current_pid);
    EXPECT_FALSE(info.command.empty());
    // The command name might be "process_manager" or "process_manager_tests" or similar
    // We'll check that it starts with "process_manager"
    EXPECT_THAT(info.command, ::testing::StartsWith("process_manager"));
}

// Test to check if GetNumCPUs returns a reasonable value
TEST_F(ProcessManagerTest, GetNumCPUsReturnsReasonableValue) {
    int num_cpus = process_manager::GetNumCPUs();
    EXPECT_GT(num_cpus, 0);
    EXPECT_LE(num_cpus, 1024); // Assuming we won't have more than 1024 CPUs in a test environment
}

// Test to check if GetUptime returns a non-negative value
TEST_F(ProcessManagerTest, GetUptimeReturnsNonNegativeValue) {
    long long uptime = process_manager::GetUptime();
    EXPECT_GE(uptime, 0);
}

// Test to check if GetSystemInfo returns valid system information
TEST_F(ProcessManagerTest, GetSystemInfoReturnsValidInformation) {
    process_manager::SystemInfo sys_info = process_manager::GetSystemInfo();

    EXPECT_GT(sys_info.num_cpus, 0);
    EXPECT_LE(sys_info.num_cpus, 1024);

    EXPECT_GT(sys_info.total_memory, 0);
    EXPECT_GT(sys_info.free_memory, 0);
    EXPECT_LE(sys_info.free_memory, sys_info.total_memory);

    EXPECT_GE(sys_info.uptime, 0);
}

// Test to check if FilterByCommand works correctly
TEST_F(ProcessManagerTest, FilterByCommandFiltersCorrectly) {
    std::vector<int> all_pids = process_manager::GetProcessList();
    // Use a common command name that is likely to be running
    std::string common_command = "bash"; // or "sh" or "init" depending on the system

    // Try to find processes with the common command
    std::vector<int> filtered_pids = process_manager::FilterByCommand(all_pids, common_command);

    // We can't guarantee that the command will be found, but if it is,
    // all filtered PIDs should have that command name
    for (int pid : filtered_pids) {
        process_manager::ProcessInfo info = process_manager::ReadProcessInfo(pid);
        EXPECT_TRUE(info.command.find(common_command) != std::string::npos || 
                    info.full_command.find(common_command) != std::string::npos);
    }
}
