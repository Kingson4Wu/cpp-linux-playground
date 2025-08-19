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

=====

# Linux C++ 实战项目路线

有，而且如果你是后端工程师出身，用 C++ 练手的项目最好能覆盖**系统编程、网络编程、多线程、性能优化**这几个核心能力，这样你既能巩固语言功底，又能学到 Linux 系统底层知识。

我帮你按**难度梯度**列了一些实用的 C++ 练手项目，每个都有学习价值和技术要点。

---

## **阶段 1：语法 & 工具熟悉期（入门级）**

目标：熟悉 C++ 现代语法、CMake 构建、调试工具链。

1. **命令行工具集**

   * 功能：实现 `ls`、`grep`、`wc` 的简化版
   * 训练点：文件 IO、字符串处理、正则表达式（`<regex>`）、CMake
   * 扩展：支持多线程处理大文件

2. **简易 JSON/YAML 解析器**

   * 功能：手写 JSON/YAML 解析
   * 训练点：STL 容器、递归下降解析、异常处理
   * 扩展：支持序列化 & 反序列化

3. **日志库（mini-spdlog）**

   * 功能：多线程安全写日志到文件
   * 训练点：多线程同步（mutex、lock\_guard）、文件缓冲、时间戳
   * 扩展：异步日志、日志轮转

---

## **阶段 2：系统编程 & 多线程（进阶）**

目标：掌握 Linux 系统调用、进程/线程管理、内存管理。

4. **多线程下载器**

   * 功能：并发下载文件（HTTP GET）
   * 训练点：pthread / C++11 thread、条件变量、socket
   * 扩展：断点续传、限速

5. **进程管理工具**

   * 功能：实现简化版 `ps` 或 `top`
   * 训练点：读取 `/proc`、系统调用、信号处理
   * 扩展：进程树可视化

6. **内存池 allocator**

   * 功能：自定义内存分配器（替代 new/delete）
   * 训练点：内存管理、对齐、free list
   * 扩展：性能测试与标准分配器对比

---

## **阶段 3：网络编程（核心）**

目标：能写出稳定、高性能的 C++ 网络服务。

7. **多线程聊天室（TCP）**

   * 功能：多个客户端互相收发消息
   * 训练点：socket、epoll、多线程同步
   * 扩展：protobuf 消息格式、命令协议解析

8. **简易 HTTP 服务器**

   * 功能：支持 GET/POST 请求，返回静态文件
   * 训练点：HTTP 协议解析、状态机、非阻塞 IO
   * 扩展：线程池、keep-alive、gzip 压缩

9. **文件传输服务器**

   * 功能：支持大文件分块传输、断点续传
   * 训练点：socket 编程、epoll、多线程、文件 IO
   * 扩展：加密传输（OpenSSL）

---

## **阶段 4：综合项目（实战）**

目标：结合 C++ 语言、系统编程、网络编程、性能优化，做成一个**可用的后端服务**。

10. **KV 存储引擎（mini-Redis）**

    * 功能：内存 KV 存储，支持 GET/SET
    * 训练点：哈希表实现、持久化（AOF/快照）、多路复用（epoll）
    * 扩展：LRU 缓存淘汰、事务、多线程

11. **搜索引擎原型（mini-Search）**

    * 功能：索引一批文本文件并支持搜索
    * 训练点：倒排索引、文件 IO、多线程构建索引
    * 扩展：HTTP API 查询接口

12. **高并发爬虫**

    * 功能：并发抓取网页并解析内容
    * 训练点：libcurl/Boost.Asio、线程池、任务队列
    * 扩展：限速、代理池、断点续抓

---

## **阶段 5：源码阅读 & 高阶优化**

目标：通过阅读优秀 C++ 项目源码，学习工程化和性能优化。

* **muduo**（高性能网络库）
* **leveldb**（Google KV 存储引擎）
* **nginx**（事件驱动模型，虽然是 C 但对 C++ 网络编程帮助很大）

====

1.  **编译项目** (如果未编译或有改动):
    ```bash
    # 配置
    cmake -S . -B build
    # 构建
    cmake --build build
    ```

2.  **运行 `my_ls` 程序**:
    可执行文件位于 `build` 目录中。从项目根目录运行以下命令：
    ```bash
    ./build/phase1/cli-tools/my_ls
    ```

运行后，你应该会看到输出：
`Hello from my_ls!`
