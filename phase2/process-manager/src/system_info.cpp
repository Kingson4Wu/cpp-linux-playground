#include "system_info.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sys/sysinfo.h>

namespace process_manager {

SystemInfo GetSystemInfo() {
    SystemInfo info{};

    info.num_cpus = GetNumCPUs();

    struct sysinfo sys_info;
    if (sysinfo(&sys_info) == 0) {
        info.total_memory = sys_info.totalram * sys_info.mem_unit / 1024; // Convert to KB
        info.free_memory = sys_info.freeram * sys_info.mem_unit / 1024;   // Convert to KB
        info.uptime = sys_info.uptime;

        // Calculate boot time
        std::time_t current_time = std::time(nullptr);
        std::time_t boot_time = current_time - sys_info.uptime;
        info.boot_time = std::chrono::system_clock::from_time_t(boot_time);
    } else {
        std::cerr << "Failed to get system information using sysinfo." << std::endl;
    }

    return info;
}

int GetNumCPUs() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

long long GetUptime() {
    struct sysinfo sys_info;
    if (sysinfo(&sys_info) == 0) {
        return sys_info.uptime;
    } else {
        std::cerr << "Failed to get uptime using sysinfo." << std::endl;
        return -1;
    }
}

} // namespace process_manager
