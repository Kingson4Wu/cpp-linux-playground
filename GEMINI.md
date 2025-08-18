# Linux C++ 后端开发实战学习项目

这是一个面向有后端开发经验工程师的 Linux C++ 学习项目，通过渐进式实战项目掌握现代 C++、系统编程、网络编程和性能优化。

## 架构说明
- **语言标准：** C++20
- **构建工具：** CMake (>= 3.20)
- **开发环境：** 跨平台（Mac/Windows/Linux）+ Docker
- **运行环境：** Linux Docker 容器
- **调试工具：** gdb/lldb + VSCode Remote-Containers

## 项目阶段

### 阶段 1：语法与工具熟悉
- 命令行工具集（ls/grep/wc 简化版）
- JSON 解析器
- 多线程日志库

### 阶段 2：系统编程
- 多线程下载器
- 进程管理工具（ps/top 简化版）
- 内存池分配器

### 阶段 3：网络编程
- TCP 多线程聊天室
- HTTP 静态文件服务器
- 文件传输服务器

### 阶段 4：综合实战
- mini-Redis（KV 存储）
- mini-Search（搜索引擎）
- 高并发爬虫

## 重要约定
- **编译标准：** `-std=c++20`
- **构建目录：** `build/`
- **源码目录：** `src/`
- **测试文件：** `*_test.cpp`
- **头文件：** `include/` 目录
- **CMake 配置：** 每个项目独立 CMakeLists.txt

## 开发环境配置

### Docker 开发容器
```dockerfile
FROM ubuntu:24.04
RUN apt update && apt install -y \
    g++ cmake git build-essential ninja-build \
    libboost-all-dev libssl-dev gdb lldb
WORKDIR /app
```

### VSCode 配置
- 必装插件：C/C++、CMake Tools、Remote-Containers
- 调试配置：支持容器内 gdb/lldb 调试
- 代码格式：clang-format

## 技术栈重点
- **现代 C++：** 智能指针、lambda、范围 for、constexpr
- **并发编程：** std::thread、std::atomic、mutex、condition_variable
- **网络编程：** socket、epoll、非阻塞 IO
- **系统调用：** 文件 IO、进程管理、内存映射
- **性能优化：** 内存池、零拷贝、锁优化

## 注意事项
- 所有项目必须在 Docker 容器内编译和运行
- 使用 CMake 管理构建，支持 Debug/Release 模式
- 重视内存安全，使用 valgrind/AddressSanitizer 检测
- 编写单元测试验证核心功能
- 每个项目完成后进行性能测试和优化

## 核心学习目标
1. 掌握现代 C++ 语法和最佳实践
2. 熟练使用 Linux 系统调用和工具链
3. 能够编写高性能多线程网络服务
4. 具备调试、性能分析和工程化能力
