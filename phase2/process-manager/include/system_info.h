#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <string>
#include <chrono>

namespace process_manager {

/**
 * @brief Structure to hold system information.
 */
struct SystemInfo {
    int num_cpus;                   ///< Number of CPU cores
    long total_memory;              ///< Total memory in KB
    long free_memory;               ///< Free memory in KB
    long long uptime;               ///< System uptime in seconds
    std::chrono::time_point<std::chrono::system_clock> boot_time; ///< System boot time
};

/**
 * @brief Gets system information.
 * 
 * @return SystemInfo containing system details.
 */
SystemInfo GetSystemInfo();

/**
 * @brief Gets the number of CPU cores.
 * 
 * @return int Number of CPU cores.
 */
int GetNumCPUs();

/**
 * @brief Gets system uptime in seconds.
 * 
 * @return long long Uptime in seconds.
 */
long long GetUptime();

} // namespace process_manager

#endif // SYSTEM_INFO_H
