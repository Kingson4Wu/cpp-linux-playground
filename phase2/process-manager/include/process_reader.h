#ifndef PROCESS_READER_H
#define PROCESS_READER_H

#include <string>
#include <vector>
#include <filesystem>

namespace process_manager {

/**
 * @brief Structure to hold process information.
 */
struct ProcessInfo {
    int pid;                ///< Process ID
    int ppid;               ///< Parent Process ID
    std::string command;    ///< Command name
    std::string full_command; ///< Full command line
    double cpu_usage;       ///< CPU usage percentage
    long memory_usage;      ///< Memory usage in KB
    std::string state;      ///< Process state (e.g., running, sleeping)
    int priority;           ///< Process priority
    int nice;               ///< Nice value
    long long start_time;   ///< Start time since boot
};

/**
 * @brief Reads process information from /proc filesystem.
 * 
 * @param pid The process ID to read information for.
 * @return ProcessInfo containing the process details.
 */
ProcessInfo ReadProcessInfo(int pid);

/**
 * @brief Gets a list of all currently running process IDs.
 * 
 * @return std::vector<int> containing all PIDs.
 */
std::vector<int> GetProcessList();

/**
 * @brief Filters a list of PIDs based on a command name.
 * 
 * @param pids The list of PIDs to filter.
 * @param command The command name to filter by.
 * @return std::vector<int> containing filtered PIDs.
 */
std::vector<int> FilterByCommand(const std::vector<int>& pids, const std::string& command);

} // namespace process_manager

#endif // PROCESS_READER_H
