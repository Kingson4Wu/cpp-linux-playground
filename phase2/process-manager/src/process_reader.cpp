#include "process_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <climits>
#include <cstring>
#include <algorithm>
#include <chrono>

namespace process_manager {

ProcessInfo ReadProcessInfo(int pid) {
    ProcessInfo info{};
    info.pid = pid;
    info.last_update = std::chrono::steady_clock::now();

#ifdef __linux__
    // Paths to /proc files
    std::string pid_str = std::to_string(pid);
    std::string stat_path = "/proc/" + pid_str + "/stat";
    std::string cmdline_path = "/proc/" + pid_str + "/cmdline";

    // Read /proc/[pid]/stat
    std::ifstream stat_file(stat_path);
    if (stat_file.is_open()) {
        std::string line;
        std::getline(stat_file, line);
        stat_file.close();

        // Parse the line
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (iss >> token) {
            tokens.push_back(token);
        }

        // Extract relevant information (index starts from 0)
        // See `man 5 proc` for the format of /proc/[pid]/stat
        if (tokens.size() > 23) {
            info.ppid = std::stoi(tokens[3]);

            // Command name is in parentheses, so we need to handle it specially
            // The command name can contain spaces, so we need to find the matching parenthesis
            size_t start = line.find('(');
            size_t end = line.rfind(')');
            if (start != std::string::npos && end != std::string::npos && end > start) {
                info.command = line.substr(start + 1, end - start - 1);
            } else {
                // Fallback if parsing fails
                info.command = tokens[1];
                // Remove parentheses
                if (!info.command.empty() && info.command.front() == '(') {
                    info.command.erase(0, 1);
                }
                if (!info.command.empty() && info.command.back() == ')') {
                    info.command.pop_back();
                }
            }

            info.state = tokens[2];
            info.priority = std::stoi(tokens[17]);
            info.nice = std::stoi(tokens[18]);
            // Store utime and stime for CPU usage calculation
            info.utime = std::stoll(tokens[13]);
            info.stime = std::stoll(tokens[14]);
            // For simplicity, we'll use a simplified CPU usage calculation for now
            // A more accurate calculation would require sampling over time
            info.cpu_usage = (info.utime + info.stime) / 100.0; // Simplified
            // Memory usage is in pages, convert to KB
            info.memory_usage = std::stol(tokens[23]) * getpagesize() / 1024;
            info.start_time = std::stoll(tokens[21]);
        }
    }

    // Read /proc/[pid]/cmdline for full command line
    std::ifstream cmdline_file(cmdline_path);
    if (cmdline_file.is_open()) {
        std::ostringstream cmdline_content;
        cmdline_content << cmdline_file.rdbuf();
        cmdline_file.close();

        // The command line arguments are null-separated, replace nulls with spaces
        std::string cmdline = cmdline_content.str();
        std::replace(cmdline.begin(), cmdline.end(), '\0', ' ');
        // Remove trailing space if present
        if (!cmdline.empty() && cmdline.back() == ' ') {
            cmdline.pop_back();
        }
        info.full_command = cmdline;
    }
#else
    // On non-Linux systems, we can't read from /proc
    // For now, we'll just set some default values
    // In a real implementation, we would use platform-specific APIs
    info.command = "unknown";
    info.full_command = "unknown";
    info.state = "unknown";
    std::cerr << "Process information reading is only supported on Linux systems." << std::endl;
#endif

    // If full_command is empty, use the command name
    if (info.full_command.empty()) {
        info.full_command = info.command;
    }

    return info;
}

std::vector<int> GetProcessList() {
    std::vector<int> pids;

#ifdef __linux__
    DIR* proc_dir = opendir("/proc");
    if (!proc_dir) {
        std::cerr << "Failed to open /proc directory." << std::endl;
        return pids;
    }

    struct dirent* entry;
    while ((entry = readdir(proc_dir)) != nullptr) {
        // Check if the entry name is a number (PID)
        char* endptr;
        long pid = strtol(entry->d_name, &endptr, 10);

        // If the entire string was a valid number
        if (*endptr == '\0' && pid > 0) {
            pids.push_back(static_cast<int>(pid));
        }
    }

    closedir(proc_dir);
#else
    // On non-Linux systems, we can't read from /proc
    // For now, we'll just return an empty list
    // In a real implementation, we would use platform-specific APIs
    std::cerr << "Process listing is only supported on Linux systems." << std::endl;
#endif

    return pids;
}

std::vector<int> FilterByCommand(const std::vector<int>& pids, const std::string& command) {
    std::vector<int> filtered_pids;

    for (int pid : pids) {
        try {
            ProcessInfo info = ReadProcessInfo(pid);
            // Check if the command name or full command line contains the search string
            if (info.command.find(command) != std::string::npos || 
                info.full_command.find(command) != std::string::npos) {
                filtered_pids.push_back(pid);
            }
        } catch (const std::exception& e) {
            // Skip processes that we can't read information for
            // This can happen if the process exits between GetProcessList and ReadProcessInfo
        }
    }

    return filtered_pids;
}

} // namespace process_manager
