---
sidebar_position: 3
---

# Process Management Tool Implementation

Build a simplified process monitoring tool similar to `ps` or `top` that can list running processes, monitor their resource usage, and provide process control capabilities. This project introduces you to Linux process management and system information retrieval.

## Project Structure

The process manager is organized as follows:

```
phase2/process-manager/
├── CMakeLists.txt
├── include/
│   └── process_manager.h  # Process manager interface and classes
└── src/
    └── process_manager.cpp  # Implementation of process management functionality
```

## Learning Objectives

During this project, you will:

- Access and parse system process information from `/proc` filesystem
- Retrieve process details like PID, command line, memory usage, and CPU time
- Monitor process resource usage in real-time
- Implement process control operations (kill, signal)
- Practice parsing structured system data
- Design an efficient interface for system monitoring

## Core Concepts

### Linux /proc Filesystem

The `/proc` filesystem provides an interface to kernel data structures. Key files and directories:

- `/proc/[pid]/`: Directory for each process ID
- `/proc/[pid]/status`: Process status information
- `/proc/[pid]/stat`: Process statistics in a single line
- `/proc/[pid]/cmdline`: Command line arguments
- `/proc/[pid]/exe`: Symbolic link to the executable
- `/proc/meminfo`: System memory information
- `/proc/cpuinfo`: CPU information
- `/proc/loadavg`: System load average

### Process Information Structure

Design a data structure to hold process information:

```cpp
struct ProcessInfo {
    pid_t pid;
    pid_t ppid;  // Parent process ID
    std::string name;
    std::string command_line;
    size_t memory_usage;  // RSS in KB
    double cpu_usage;     // Percentage
    std::string state;    // Running, sleeping, etc.
    uid_t user_id;
    std::chrono::steady_clock::time_point start_time;
};
```

## Implementation Architecture

### ProcessManager Interface

```cpp
class ProcessManager {
public:
    static std::vector<ProcessInfo> get_all_processes();
    static std::optional<ProcessInfo> get_process_by_pid(pid_t pid);
    static std::vector<ProcessInfo> filter_processes(const std::string& name_filter);
    
    static bool kill_process(pid_t pid, int signal = SIGTERM);
    static bool send_signal(pid_t pid, int signal);
    static std::vector<ProcessInfo> get_processes_by_user(uid_t uid);
    
    static SystemInfo get_system_info();
};
```

### System Information Structure

```cpp
struct SystemInfo {
    size_t total_memory;
    size_t free_memory;
    size_t total_swap;
    size_t free_swap;
    double load_average[3];  // 1, 5, and 15 minute averages
    int num_running_processes;
    int num_total_processes;
};
```

### Sample Implementation Approach

```cpp
class ProcessManager {
private:
    static ProcessInfo parse_proc_status(const std::string& pid);
    static ProcessInfo parse_proc_stat(const std::string& pid);
    static std::string get_cmdline(const std::string& pid);
    
public:
    static std::vector<ProcessInfo> get_all_processes();
    static std::chrono::milliseconds get_process_uptime(const ProcessInfo& proc);
};
```

## Implementation Requirements

### Core Features

1. **Process Listing**: Retrieve and display information about all running processes
2. **Search and Filter**: Find processes by name, PID, or user
3. **Process Control**: Send signals to processes (SIGTERM, SIGKILL, etc.)
4. **Resource Monitoring**: Show memory and CPU usage
5. **Real-time Updates**: Refresh process information periodically

### Information to Collect

For each process, gather:

- Process ID (PID) and parent process ID (PPID)
- Process name and full command line
- Memory usage (virtual memory and resident set size)
- Execution state (running, sleeping, stopped, etc.)
- Start time and accumulated CPU time
- User ID and group ID of the process owner
- File descriptors count (optional)

### System Information

Display system-level information:

- Total and available memory
- Load averages
- Total number of processes
- CPU information

## Performance Considerations

- Cache process information to avoid excessive system calls
- Use efficient string parsing for `/proc` files
- Consider using `inotify` for monitoring changes (advanced)
- Implement pagination for systems with many processes

## Testing Strategy

Test your process manager with:

- Retrieving information for all processes
- Finding specific processes by name or PID
- Getting system information
- Verifying process control operations on test processes
- Error handling for non-existent PIDs

## Security Considerations

- Respect Linux permissions - you may not be able to access all process information
- Handle permission errors gracefully
- Note that some information may not be accessible for processes owned by other users

## Usage Example

```cpp
#include "process_manager.h"
#include <iostream>

int main() {
    // Get all processes
    auto processes = ProcessManager::get_all_processes();
    
    std::cout << "Process ID\tName\t\tMemory (KB)\tCommand" << std::endl;
    for (const auto& proc : processes) {
        std::cout << proc.pid << "\t\t" 
                  << proc.name << "\t\t" 
                  << proc.memory_usage << "\t\t" 
                  << proc.command_line.substr(0, 50) << "..." << std::endl;
    }
    
    // Find processes by name
    auto grep_processes = ProcessManager::filter_processes("grep");
    for (const auto& proc : grep_processes) {
        std::cout << "Found grep process: " << proc.pid << std::endl;
    }
    
    // Get system information
    auto sys_info = ProcessManager::get_system_info();
    std::cout << "Memory - Total: " << sys_info.total_memory 
              << "KB, Free: " << sys_info.free_memory << "KB" << std::endl;
    
    return 0;
}
```

## Building and Testing

```bash
# Build the project
cd build
cmake ..
make -j4

# The process manager should be integrated into the main build
# Run inside the Docker container to access Linux-specific features
```

## Next Steps

After completing the process manager, implement the [Memory Pool Allocator](./memory-pool.md) project to explore custom memory management.