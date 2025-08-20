# 开发环境问题排查记录

## 1. VSCode中"无法打开源文件 'gtest/gtest.h'"错误

### 问题描述
在VSCode中开发时，遇到无法找到Google Test头文件的错误，导致IntelliSense无法正确识别gtest/gtest.h头文件。

### 问题原因
- Google Test通过CMake的FetchContent机制动态下载和构建
- VSCode的IntelliSense不知道这些动态下载的头文件位置
- 需要正确配置包含路径

### 解决方案
1. 重新配置CMake以生成编译命令数据库：
   ```bash
   cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
   ```

2. 创建或更新`.vscode/c_cpp_properties.json`文件，添加正确的包含路径：
   ```json
   {
       "configurations": [
           {
               "name": "Linux",
               "includePath": [
                   "${workspaceFolder}/**",
                   "${workspaceFolder}/build/_deps/googletest-src/googletest/include",
                   "${workspaceFolder}/build/_deps/googletest-src/googlemock/include",
                   "${workspaceFolder}/phase1/cli-tools/include"
               ],
               "defines": [],
               "compilerPath": "/usr/bin/g++",
               "cStandard": "c17",
               "cppStandard": "c++20",
               "intelliSenseMode": "linux-gcc-x64",
               "compileCommands": "${workspaceFolder}/build/compile_commands.json"
           }
       ],
       "version": 4
   }
   ```

3. 为了让VSCode重新加载配置，可以执行以下操作之一：
   - 重新打开VSCode窗口
   - 按F1，然后选择"C/C++: Edit Configurations (UI)"
   - 按F1，然后选择"C/C++: Reload Window"

### 其他相关配置
为了改善开发体验，还创建了以下配置文件：
- `.vscode/tasks.json`：定义了构建和运行测试的任务
- `.vscode/launch.json`：定义了调试配置

完成这些步骤后，VSCode应该能够正确识别gtest/gtest.h头文件，错误提示应该消失。

## 2. Docker开发环境相关问题

### 问题描述
在使用Docker开发环境时可能遇到的常见问题和解决方案。

### 构建和运行
- 确保Docker和Docker Compose已正确安装
- 使用`./scripts/docker-dev.sh build`构建镜像
- 使用`./scripts/docker-dev.sh run`启动容器
- 使用`./scripts/docker-dev.sh exec`进入容器

### CMake缓存问题
如果遇到CMake缓存相关的错误，可以：
1. 删除build目录中的缓存文件
2. 使用`find . -name CMakeCache.txt -delete`删除缓存文件
3. 重新运行CMake配置
