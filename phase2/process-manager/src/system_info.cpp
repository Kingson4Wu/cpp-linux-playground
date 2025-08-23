#include "system_info.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <unistd.h>

#ifdef __linux__
#include <sys/sysinfo.h>
#elif __APPLE__
#include <mach/mach.h>
#include <mach/host_info.h>
#include <mach/mach_host.h>
#include <sys/sysctl.h>
#endif

namespace process_manager {

SystemInfo GetSystemInfo() {
    SystemInfo info{};

    info.num_cpus = GetNumCPUs();

#ifdef __linux__
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
#elif __APPLE__
    // Get memory information on macOS
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t info_count = HOST_VM_INFO64_COUNT;
    kern_return_t kern_result = host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&vm_stats, &info_count);

    if (kern_result == KERN_SUCCESS) {
        uint64_t page_size;
        size_t page_size_len = sizeof(page_size);
        sysctlbyname("hw.pagesize", &page_size, &page_size_len, NULL, 0);

        info.total_memory = (vm_stats.active_count + vm_stats.inactive_count + vm_stats.wire_count + vm_stats.free_count) * page_size / 1024;
        info.free_memory = vm_stats.free_count * page_size / 1024;
    } else {
        std::cerr << "Failed to get memory information on macOS." << std::endl;
    }

    // Get uptime on macOS
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};
    if (sysctl(mib, 2, &boottime, &len, NULL, 0) == 0) {
        std::time_t current_time = std::time(nullptr);
        std::time_t boot_time_sec = boottime.tv_sec;
        info.uptime = current_time - boot_time_sec;
        info.boot_time = std::chrono::system_clock::from_time_t(boot_time_sec);
    } else {
        std::cerr << "Failed to get uptime information on macOS." << std::endl;
    }
#endif

    return info;
}

int GetNumCPUs() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

long long GetUptime() {
#ifdef __linux__
    struct sysinfo sys_info;
    if (sysinfo(&sys_info) == 0) {
        return sys_info.uptime;
    } else {
        std::cerr << "Failed to get uptime using sysinfo." << std::endl;
        return -1;
    }
#elif __APPLE__
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = {CTL_KERN, KERN_BOOTTIME};
    if (sysctl(mib, 2, &boottime, &len, NULL, 0) == 0) {
        std::time_t current_time = std::time(nullptr);
        return current_time - boottime.tv_sec;
    } else {
        std::cerr << "Failed to get uptime information on macOS." << std::endl;
        return -1;
    }
#endif
}

} // namespace process_manager
