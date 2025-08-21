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
- **源码目录：** 按阶段组织（如 `phase1/cli-tools/src/`）
- **测试文件：** `*_test.cpp`
- **头文件：** `include/` 目录
- **CMake 配置：** 每个项目独立 CMakeLists.txt
- **文档规范：** 
  - 每个可执行子项目的主源文件头部必须包含编译、运行和使用方法的注释说明
  - 每个库的实现文件也需要包含使用说明和运行方法
  - 测试文件需要包含如何运行测试的说明
- **开发规范：**
  - 所有代码注释和说明必须使用英文
  - 不要擅自提交代码到Git，所有更改需要经过审查

## 开发环境配置

### Docker 开发容器
```dockerfile
FROM ubuntu:24.04
RUN apt update && apt install -y \
    g++ cmake git build-essential ninja-build \
    libboost-all-dev libssl-dev gdb lldb clang-format valgrind
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

## 代码规范

### 文件组织
- 每个子项目包含独立的 `src` 和 `include` 目录
- 库实现放在 `src` 目录下
- 头文件放在 `include` 目录下
- 测试文件放在 `tests` 目录下，按项目阶段组织

### 注释规范
- 每个源文件头部必须包含文件说明和使用方法
- 重要的函数需要添加注释说明功能和参数
- 复杂的逻辑需要添加注释解释实现思路

### CMake 规范
- 根 CMakeLists.txt 管理所有子项目
- 每个子项目包含独立的 CMakeLists.txt
- 库使用 `add_library` 创建
- 可执行文件使用 `add_executable` 创建
- 使用 `target_link_libraries` 链接依赖

### 测试规范
- 使用 Google Test 进行单元测试
- 每个测试文件需要包含如何运行测试的说明
- 测试用例需要覆盖正常和异常情况

## 项目整体计划

- [x] **阶段 0：项目初始化**
  - [x] 初始化 Git 仓库
  - [x] 配置 `.gitignore`
  - [x] 搭建基础 CMake 构建框架
  - [x] 编写项目说明 `GEMINI.md`
- [ ] **阶段 1：语法与工具熟悉**
  - [ ] 命令行工具集 (cli-tools)
    - [x] `ls`
    - [x] `grep`
    - [ ] `wc`
  - [ ] JSON 解析器 (json-parser)
  - [ ] 多线程日志库 (logger)
- [ ] **阶段 2：系统编程**
  - [ ] 多线程下载器 (downloader)
  - [ ] 进程管理工具 (proc-manager)
  - [ ] 内存池分配器 (memory-pool)
- [ ] **阶段 3：网络编程**
  - [ ] TCP 多线程聊天室 (chat-room)
  - [ ] HTTP 静态文件服务器 (http-server)
  - [ ] 文件传输服务器 (file-transfer)
- [ ] **阶段 4：综合实战**
  - [ ] mini-Redis（kv-store）
  - [ ] mini-Search（search-engine）
  - [ ] 高并发爬虫 (web-crawler)

## 当前进度
项目当前处于 **阶段 1：语法与工具熟悉**，已完成 **命令行工具集** 中的 `ls` 和 `grep` 命令。

## 目录结构规划
为了保持项目清晰和可扩展性，约定以下目录结构。源代码按项目阶段进行组织，每个子项目（如 `cli-tools`）都包含独立的 `src` 和 `include` 目录，以实现更好的模块化。
```
.
├── CMakeLists.txt      # 根 CMake 配置，管理所有子项目
├── build/              # 编译产物目录 (不提交到 Git)
├── docs/               # 项目文档
├── docker/             # Docker 相关配置，如 Dockerfile
├── phase1/             # 阶段 1：语法与工具熟悉
│   ├── CMakeLists.txt  # 聚合 phase1 的项目
│   ├── cli-tools/
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   │   ├── grep.h
│   │   │   └── ls.h
│   │   └── src/
│   │       ├── grep_lib.cpp
│   │       ├── grep_main.cpp
│   │       ├── ls_lib.cpp
│   │       └── ls_main.cpp
│   ├── json-parser/
│   └── logger/
├── phase2/             # 阶段 2：系统编程
├── phase3/             # 阶段 3：网络编程
├── phase4/             # 阶段 4：综合实战
├── scripts/            # 辅助脚本 (构建、测试、运行)
├── tests/              # 测试文件
│   └── phase1/
│       └── cli-tools/
│           ├── CMakeLists.txt
│           └── ls_test.cpp
└── third_party/        # 第三方依赖库
```

## 运行方式

### 使用 Docker 开发脚本
```bash
# 构建 Docker 镜像
./scripts/docker-dev.sh build

# 启动容器
./scripts/docker-dev.sh run

# 进入容器
./scripts/docker-dev.sh exec

# 构建并运行 my_ls
./scripts/docker-dev.sh run-ls

# 构建并运行 my_grep
./scripts/docker-dev.sh run-grep

# 运行测试
./scripts/docker-dev.sh test

# 调试测试
./scripts/docker-dev.sh debug

# 停止容器
./scripts/docker-dev.sh stop

# 清理容器和卷
./scripts/docker-dev.sh clean
```

### 手动编译运行
```bash
# 从项目根目录
cmake -S . -B build
cmake --build build -- -j
./build/phase1/cli-tools/my_ls [path]
```

### VSCode 开发容器
1. 安装 "Dev Containers" 和 "CMake Tools" 插件
2. 打开项目在开发容器中 (VS Code 会自动 attach 到 Docker)
3. CMake Tools 会自动配置项目 (cmake -S . -B build)
4. 在底部状态栏选择目标 (如 "my_ls")
5. 按 F5 构建并调试容器内程序
