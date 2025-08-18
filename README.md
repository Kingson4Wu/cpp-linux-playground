# cpp-linux-playground
A hands-on C++ learning playground for Linux, covering system programming, networking, multithreading, and performance optimization.

# cpp-linux-playground

> 🐧 Linux C++ 实战项目路线 | 系统编程 & 网络编程 & 高性能练手合集

这是一个面向有后端开发经验的工程师的 **C++ Linux 实战练手项目合集**。  
重点覆盖：**现代 C++ 语法、系统编程、并发、多线程、网络编程、调试与工程化能力**。

---

## 🎯 项目目标

通过一系列分阶段的实战项目，让你：

- 掌握现代 C++（C++11/14/17/20）核心特性  
- 熟练 Linux 系统调用、文件 IO、进程与线程管理  
- 能写高性能网络服务（TCP/HTTP、多路复用）  
- 学会性能优化与工程化（内存池、锁优化、日志库、构建工具）  
- 积累一整套可展示的后端 C++ 作品集  

---

## 🚀 项目路线

### **阶段 1：语法 & 工具熟悉（入门）**
- 命令行工具集：实现 `ls` / `grep` / `wc` 简化版  
- 简易 JSON/YAML 解析器  
- 多线程日志库（mini-spdlog）

### **阶段 2：系统编程 & 多线程（进阶）**
- 多线程下载器（支持断点续传）  
- 进程管理工具（简化 ps/top）  
- 内存池 allocator（自定义内存分配器）

### **阶段 3：网络编程（核心）**
- 多线程聊天室（TCP 长连接）  
- 简易 HTTP 服务器（GET/POST，线程池）  
- 文件传输服务器（大文件分块传输、断点续传）

### **阶段 4：综合项目（实战）**
- mini-Redis（内存 KV 存储，持久化，epoll 多路复用）  
- mini-Search（倒排索引搜索引擎）  
- 高并发爬虫（异步网络 IO + 任务队列）

### **阶段 5：源码阅读 & 性能优化**
- 阅读 `muduo`、`leveldb`、`nginx` 源码  
- 内存池、锁优化、缓存友好数据结构  

---

## 🛠 技术栈

- **语言**：C++11/14/17/20  
- **平台**：Linux (推荐 Ubuntu / Debian)  
- **构建工具**：CMake / Makefile  
- **调试分析**：GDB / Valgrind / Perf / AddressSanitizer  
- **网络**：Socket API / epoll / Reactor 模型  

---

## 📖 学习资源

- 《Effective C++》《More Effective C++》— Scott Meyers  
- 《C++ Primer（第5版）》  
- 《Advanced Programming in the UNIX Environment》— W. Richard Stevens  
- 《UNIX Network Programming》— W. Richard Stevens  
- 《Linux 高性能服务器编程》— 游双  
- [CppReference](https://en.cppreference.com)  

---

## 💡 使用方法

```bash
# 克隆仓库
git clone https://github.com/你的用户名/cpp-linux-playground.git
cd cpp-linux-playground

# 构建示例项目
mkdir build && cd build
cmake ..
make -j

# 运行示例
./bin/项目名
