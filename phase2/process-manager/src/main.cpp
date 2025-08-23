/**
 * @file main.cpp
 * @brief Main entry point for the process manager application (my_ps).
 *
 * This application allows users to view information about running processes,
 * similar to the 'ps' command in Linux. It can display a snapshot of current
 * processes or continuously monitor them in a 'top'-like view.
 *
 * How to Run with Docker (builds and runs automatically):
 *   ./scripts/docker-dev.sh exec
 *   # Inside the container, navigate to the build directory and run the example
 *   cd build/phase2/process-manager
 *   ./my_ps [OPTIONS]
 *
 * How to Compile and Run manually in Docker:
 *   1. Enter the Docker container:
 *      ./scripts/docker-dev.sh exec
 *   2. Inside the container, create a build directory and compile:
 *      mkdir -p build && cd build
 *      cmake ..
 *      make
 *   3. Run the executable:
 *      ./phase2/process-manager/my_ps [OPTIONS]
 *
 * How to Compile without Docker (from project root):
 *   1. cmake -S . -B build
 *   2. cmake --build build -- -j
 *
 * How to Run without Docker:
 *   ./build/phase2/process-manager/my_ps [OPTIONS]
 *
 * Usage:
 *   ./build/phase2/process-manager/my_ps [OPTIONS]
 *
 * Options:
 *   -a, --all              Show all processes (default: only current user's processes)
 *   -f, --full             Show full format listing
 *   -p, --pid PID          Show only the process with the specified PID
 *   -c, --command CMD      Show only processes with the specified command name
 *   -t, --top              Continuously monitor processes (top-like view)
 *   -n, --interval SEC     Set the refresh interval for top mode (default: 1.0 seconds)
 *   -h, --help             Show this help message
 *
 * Examples:
 *   # List all processes
 *   ./build/phase2/process-manager/my_ps -a
 *
 *   # List processes with full format
 *   ./build/phase2/process-manager/my_ps -f
 *
 *   # Show only process with PID 1234
 *   ./build/phase2/process-manager/my_ps -p 1234
 *
 *   # Show processes with command name 'bash'
 *   ./build/phase2/process-manager/my_ps -c bash
 *
 *   # Continuously monitor processes (top-like view) with 2-second interval
 *   ./build/phase2/process-manager/my_ps -t -n 2.0
 *
 * Debugging with VS Code Dev Container + CMake Tools:
 *   1. Install the "Dev Containers" and "CMake Tools" extensions in VS Code.
 *   2. Open the project in a Dev Container (VS Code will attach into Docker).
 *   3. CMake Tools automatically configures the project (cmake -S . -B build).
 *   4. Select the target (e.g., "my_ps") in the bottom status bar.
 *   5. Press F5 to build and debug inside the container.
 */

#include "process_reader.h"
#include "system_info.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <getopt.h>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cmath>

// Function to print help message
void PrintHelp(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [OPTIONS]
"
              << "Display information about running processes.

"
              << "Options:
"
              << "  -a, --all              Show all processes
"
              << "  -f, --full             Show full format listing
"
              << "  -p, --pid PID          Show only the process with the specified PID
"
              << "  -c, --command CMD      Show only processes with the specified command name
"
              << "  -t, --top              Continuously monitor processes (top-like view)
"
              << "  -n, --interval SEC     Set the refresh interval for top mode (default: 1.0 seconds)
"
              << "  -h, --help             Show this help message
";
}

// Function to print system information
void PrintSystemInfo(const process_manager::SystemInfo& sys_info) {
    std::time_t boot_time = std::chrono::system_clock::to_time_t(sys_info.boot_time);
    std::cout << "System Info:
";
    std::cout << "  CPUs: " << sys_info.num_cpus << "
";
    std::cout << "  Total Memory: " << sys_info.total_memory << " KB
";
    std::cout << "  Free Memory: " << sys_info.free_memory << " KB
";
    std::cout << "  Uptime: " << sys_info.uptime << " seconds
";
    std::cout << "  Boot Time: " << std::put_time(std::localtime(&boot_time), "%Y-%m-%d %H:%M:%S") << "

";
}

// Function to print process information header
void PrintProcessHeader(bool full_format) {
    if (full_format) {
        std::cout << std::left << std::setw(8) << "PID" 
                  << std::left << std::setw(8) << "PPID" 
                  << std::left << std::setw(10) << "CPU%" 
                  << std::left << std::setw(10) << "MEM(KB)" 
                  << std::left << std::setw(8) << "PRI" 
                  << std::left << std::setw(8) << "NI" 
                  << std::left << std::setw(10) << "STATE" 
                  << "COMMAND
";
    } else {
        std::cout << std::left << std::setw(8) << "PID" 
                  << std::left << std::setw(10) << "CPU%" 
                  << std::left << std::setw(10) << "MEM(KB)" 
                  << "COMMAND
";
    }
}

// Function to print process information
void PrintProcessInfo(const process_manager::ProcessInfo& info, bool full_format) {
    if (full_format) {
        std::cout << std::left << std::setw(8) << info.pid
                  << std::left << std::setw(8) << info.ppid
                  << std::left << std::setw(10) << std::fixed << std::setprecision(2) << info.cpu_usage
                  << std::left << std::setw(10) << info.memory_usage
                  << std::left << std::setw(8) << info.priority
                  << std::left << std::setw(8) << info.nice
                  << std::left << std::setw(10) << info.state
                  << info.full_command << "
";
    } else {
        std::cout << std::left << std::setw(8) << info.pid
                  << std::left << std::setw(10) << std::fixed << std::setprecision(2) << info.cpu_usage
                  << std::left << std::setw(10) << info.memory_usage
                  << info.command << "
";
    }
}

// Function to get a list of PIDs based on options
std::vector<int> GetPIDs(bool show_all, int specific_pid, const std::string& command_filter) {
    std::vector<int> pids;

    if (specific_pid > 0) {
        // Show only the specified PID
        pids.push_back(specific_pid);
    } else if (!command_filter.empty()) {
        // Filter by command name
        std::vector<int> all_pids = process_manager::GetProcessList();
        pids = process_manager::FilterByCommand(all_pids, command_filter);
    } else {
        // Show all processes or only current user's processes
        pids = process_manager::GetProcessList();
        if (!show_all) {
            // For simplicity, we'll show all processes even when -a is not specified
            // In a real 'ps' implementation, we would filter by user
            // This is a simplified version
        }
    }

    return pids;
}

// Function to display processes once
void DisplayProcesses(bool show_all, bool full_format, int specific_pid, const std::string& command_filter) {
    std::vector<int> pids = GetPIDs(show_all, specific_pid, command_filter);

    // Sort PIDs for consistent output
    std::sort(pids.begin(), pids.end());

    // Print header
    PrintProcessHeader(full_format);

    // Print process information
    for (int pid : pids) {
        try {
            process_manager::ProcessInfo info = process_manager::ReadProcessInfo(pid);
            PrintProcessInfo(info, full_format);
        } catch (const std::exception& e) {
            // Skip processes that we can't read information for
            // This can happen if the process exits between GetProcessList and ReadProcessInfo
        }
    }
}

// Function for continuous monitoring (top-like view)
void MonitorProcesses(bool show_all, bool full_format, double interval) {
    while (true) {
        // Clear screen (Unix/Linux)
        std::cout << "\033[2J\033[1;1H"; // ANSI escape code to clear screen and move cursor to top-left

        // Print system information
        process_manager::SystemInfo sys_info = process_manager::GetSystemInfo();
        PrintSystemInfo(sys_info);

        // Get and sort PIDs
        std::vector<int> pids = GetPIDs(show_all, -1, ""); // -1 means no specific PID
        std::sort(pids.begin(), pids.end());

        // For a simple top-like view, let's sort by CPU usage (descending)
        std::vector<process_manager::ProcessInfo> processes;
        for (int pid : pids) {
            try {
                process_manager::ProcessInfo info = process_manager::ReadProcessInfo(pid);
                processes.push_back(info);
            } catch (const std::exception& e) {
                // Skip processes that we can't read information for
            }
        }

        // Sort by CPU usage
        std::sort(processes.begin(), processes.end(), 
                  [](const process_manager::ProcessInfo& a, const process_manager::ProcessInfo& b) {
                      return a.cpu_usage > b.cpu_usage;
                  });

        // Limit to top 20 processes for display
        size_t display_count = std::min(static_cast<size_t>(20), processes.size());

        // Print header
        PrintProcessHeader(full_format);

        // Print top processes
        for (size_t i = 0; i < display_count; ++i) {
            PrintProcessInfo(processes[i], full_format);
        }

        // Sleep for the specified interval
        std::this_thread::sleep_for(std::chrono::duration<double>(interval));
    }
}

int main(int argc, char* argv[]) {
    // Default values
    bool show_all = false;
    bool full_format = false;
    bool top_mode = false;
    double interval = 1.0;
    int specific_pid = -1;
    std::string command_filter;

    // Define long options
    static struct option long_options[] = {
        {"all", no_argument, 0, 'a'},
        {"full", no_argument, 0, 'f'},
        {"pid", required_argument, 0, 'p'},
        {"command", required_argument, 0, 'c'},
        {"top", no_argument, 0, 't'},
        {"interval", required_argument, 0, 'n'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "afp:c:tn:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'a':
                show_all = true;
                break;
            case 'f':
                full_format = true;
                break;
            case 'p':
                try {
                    specific_pid = std::stoi(optarg);
                    if (specific_pid <= 0) {
                        std::cerr << "Error: PID must be a positive integer." << std::endl;
                        return 1;
                    }
                } catch (const std::exception&) {
                    std::cerr << "Error: Invalid PID specified." << std::endl;
                    return 1;
                }
                break;
            case 'c':
                command_filter = optarg;
                break;
            case 't':
                top_mode = true;
                break;
            case 'n':
                try {
                    interval = std::stod(optarg);
                    if (interval <= 0) {
                        std::cerr << "Error: Interval must be a positive number." << std::endl;
                        return 1;
                    }
                } catch (const std::exception&) {
                    std::cerr << "Error: Invalid interval specified." << std::endl;
                    return 1;
                }
                break;
            case 'h':
                PrintHelp(argv[0]);
                return 0;
            default:
                std::cerr << "Unknown option. Use -h for help." << std::endl;
                return 1;
        }
    }

    // Check for conflicting options
    if (specific_pid > 0 && !command_filter.empty()) {
        std::cerr << "Error: Cannot specify both -p and -c options." << std::endl;
        return 1;
    }

    if (top_mode && (specific_pid > 0 || !command_filter.empty())) {
        std::cerr << "Error: Cannot use -t option with -p or -c options." << std::endl;
        return 1;
    }

    try {
        if (top_mode) {
            MonitorProcesses(show_all, full_format, interval);
        } else {
            DisplayProcesses(show_all, full_format, specific_pid, command_filter);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
