# Linux C++ 后端开发实战学习项目

这是一个面向有后端开发经验工程师的 Linux C++ 学习项目，通过渐进式实战项目掌握现代 C++、系统编程、网络编程和性能优化。

## 项目概述

### 核心特点
- **语言标准**: C++20
- **构建工具**: CMake (>= 3.20)
- **开发环境**: 跨平台（Mac/Windows/Linux）+ Docker
- **运行环境**: Linux Docker 容器
- **调试工具**: gdb/lldb + VSCode Remote-Containers

## 项目阶段与学习路径

### 阶段 1：语法与工具熟悉
- 命令行工具集（ls/grep/wc 简化版）
- JSON 解析器
- 多线程日志库

### 阶段 2：系统编程
目标：掌握Linux系统调用、进程/线程管理、内存管理
- 多线程下载器
- 进程管理工具（ps/top 简化版）
- 内存池分配器

### 阶段 3：网络编程
目标：能写出稳定、高性能的C++网络服务
- TCP 多线程聊天室
- HTTP 静态文件服务器
- 文件传输服务器

### 阶段 4：综合实战
目标：结合所有技能，做成可用的后端服务
- mini-Redis（KV 存储）
- mini-Search（搜索引擎）
- 高并发爬虫

## 重要约定与规范

- **编译标准**: `-std=c++20`
- **构建目录**: `build/`
- **源码目录**: 按阶段组织（如 `phase1/`）
- **测试文件**: `*_test.cpp`
- **头文件**: `include/` 目录
- **CMake 配置**: 每个项目独立 CMakeLists.txt
- **文档规范**: 
  - 每个可执行子项目的主源文件头部必须包含编译、运行和使用方法的注释说明
  - 每个库的实现文件也需要包含使用说明和运行方法
  - 测试文件需要包含如何运行测试的说明
- **开发规范**:
  - 所有代码注释和说明必须使用英文
  - 除非有用户的明确指令，否则不要提交代码变动到 Git
  - 执行任何任务前，必须先输出具体的实施方案，等待用户确认后才能按方案执行
- **质量保证**:
  - 编写单元测试验证核心功能
  - 使用代码覆盖率工具确保测试质量（目标覆盖率 > 90%）
  - 重视内存安全，使用 valgrind/AddressSanitizer 检测

## Git 提交规范

为了保持提交历史的清晰和一致性，我们遵循专业的 Git 提交规范：

### 提交信息格式
```
<type>(<scope>): <subject>

<body>

<footer>
```

### 提交类型 (Type)
- `feat`: 新功能
- `fix`: 修复 bug
- `bug`: 修复 bug (与 `fix` 同义)
- `docs`: 文档更新
- `style`: 代码格式调整 (不影响代码含义的更改，如空格、格式化等)
- `refactor`: 代码重构 (既不修复 bug 也不添加功能的代码更改)
- `perf`: 性能优化
- `test`: 测试相关
- `build`: 构建系统或外部依赖的更改
- `ci`: CI 配置文件和脚本的更改
- `chore`: 其他不修改 src 或测试文件的更改
- `revert`: 回滚之前的提交

### 范围 (Scope)
范围是可选的，用于标识提交影响的模块或组件，如:
- `cli-tools`
- `grep`
- `ls`
- `cmake`
- `docker`
- `tests`

### 提交信息规范
1. 使用英文编写提交信息
2. 第一行是简短描述 (<72 字符)
3. 第二行为空行
4. 第三行开始是详细描述 (可选)
5. 详细描述中可以包含变更原因、影响范围等信息

### 示例
```
feat(cli-tools): 实现 grep 命令的基本功能

- 添加 grep_lib.cpp 库实现文件
- 添加 grep_main.cpp 主程序文件
- 实现文件搜索和模式匹配功能

Resolves: #123
```

```
fix(grep): 修复 Docker 开发脚本中的参数传递问题

- 修复 run-grep 命令无法传递参数给可执行文件的问题
- 调整工作目录使 grep 命令能正确访问项目根目录下的文件
- 添加 shift 命令以正确处理传递给脚本的参数
```

## 技术栈重点

- **现代 C++：** 智能指针、lambda、范围 for、constexpr
- **并发编程：** std::thread、std::atomic、mutex、condition_variable
- **网络编程：** socket、epoll、非阻塞 IO
- **系统调用：** 文件 IO、进程管理、内存映射
- **性能优化：** 内存池、零拷贝、锁优化

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

## 注意事项与最佳实践

1. 所有项目必须在 Docker 容器内编译和运行
2. 使用 CMake 管理构建，支持 Debug/Release 模式
3. 编写单元测试验证核心功能
4. 使用代码覆盖率工具确保测试质量（目标覆盖率 > 90%）
5. 重视内存安全，使用 valgrind/AddressSanitizer 检测
6. 每个项目完成后进行性能测试和优化

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
- 测试通过标准：所有测试用例必须 100% 通过，且命令返回码为 0。具体请参考 `docs/test_pass_criteria.md`。

## 目录结构规划

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
│   │   │   ├── ls.h
│   │   │   └── wc.h
│   │   └── src/
│   │       ├── grep_lib.cpp
│   │       ├── grep_main.cpp
│   │       ├── ls_lib.cpp
│   │       ├── ls_main.cpp
│   │       ├── wc_lib.cpp
│   │       └── wc_main.cpp
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

## 学习建议

1. **循序渐进**：按照阶段顺序学习，每个阶段的项目都有其特定的学习目标
2. **动手实践**：不仅要阅读代码，更要亲手实现和调试
3. **关注性能**：在实现功能的基础上，注重性能优化和内存管理
4. **测试驱动**：为关键功能编写单元测试，确保代码质量
5. **源码阅读**：项目后期可以阅读muduo、leveldb等优秀开源项目的源码

## 当前进度

项目当前处于 **阶段 3：网络编程**，已完成 **TCP 多线程聊天室**、**HTTP 静态文件服务器**。

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

# 构建并运行 my_wc
./scripts/docker-dev.sh run-wc

# 运行测试 (使用独立的 build-test 目录)
./scripts/docker-dev.sh test

# 调试测试 (使用独立的 build 目录)
./scripts/docker-dev.sh debug

# 停止容器
./scripts/docker-dev.sh stop

# 清理容器和卷
./scripts/docker-dev.sh clean
```

**注意**: 为了提高开发效率，不同的任务使用了独立的构建目录：
*   `run-*` 和 `debug` 命令使用 `build/` 目录。
*   `test` 命令使用 `build-test/` 目录。
*   `coverage` 命令使用 `build_coverage/` 目录。
这样可以避免在不同任务之间切换时进行不必要的重新编译。

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
3. CMake Tools 自动配置项目 (cmake -S . -B build)
4. 在底部状态栏选择目标 (如 "my_ls")
5. 按 F5 构建并调试容器内程序

### GitHub Actions 持续集成
项目配置了 GitHub Actions 工作流，在每次推送到 `main` 分支或创建拉取请求时自动运行单元测试。

工作流配置文件位于 `.github/workflows/ci.yml`，包含以下步骤：
1. 在 Ubuntu 24.04 环境中设置构建环境
2. 安装必要的依赖项（g++, cmake, build-essential等）
3. 配置和构建项目
4. 运行所有单元测试

可以通过 GitHub 仓库的 "Actions" 标签页查看构建和测试结果。

### 代码覆盖率
项目支持生成代码覆盖率报告，以确保测试质量：

```bash
# 使用 Docker 生成覆盖率报告
./scripts/docker-dev.sh coverage
```

覆盖率报告将生成在 `build_coverage/coverage/` 目录中：
- HTML 格式报告：`build_coverage/coverage/index.html`
- XML 格式报告：`build_coverage/coverage/coverage.xml`

目标是达到 90% 以上的代码覆盖率，以确保核心功能得到充分测试。

通过这个项目的学习，开发者可以系统地掌握Linux C++后端开发的核心技能，为将来从事高性能后端开发工作打下坚实的基础。
