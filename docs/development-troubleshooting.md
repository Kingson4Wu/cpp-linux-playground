## 解决VSCode中"无法打开源文件 'gtest/gtest.h'"错误的步骤

现在让我总结一下解决VSCode中"无法打开源文件 'gtest/gtest.h'"错误的步骤：1. 问题原因：
   - Google Test是通过CMake的FetchContent机制动态下载和构建的   - VSCode的IntelliSense不知道这些头文件的位置   - 需要正确配置包含路径

2. 解决方案： 
    a. 重新配置CMake以生成编译命令数据库：      
    ```bash      
    cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    ```
    b. 创建或更新`.vscode/c_cpp_properties.json`文件，添加正确的包含路径： 
    ```json
        {          "configurations": [ {
                    "name": "Linux",
                    "includePath": [                      "${workspaceFolder}/**",
                        "${workspaceFolder}/build/_deps/googletest-src/googletest/include",
                        "${workspaceFolder}/build/_deps/googletest-src/googlemock/include",                      "${workspaceFolder}/phase1/cli-tools/include"
                    ],                  "defines": [],
                    "compilerPath": "/usr/bin/g++",
                    "cStandard": "c17", "cppStandard": "c++20",
                    "intelliSenseMode": "linux-gcc-x64",                  "compileCommands": "${workspaceFolder}/build/compile_commands.json"
                } ],
            "version": 4      
            }
    ```
    c. 为了让VSCode重新加载配置，可以执行以下操作之一：      
        - 重新打开VSCode窗口      
        - 按F1，然后选择"C/C++: Edit Configurations (UI)"      
        - 按F1，然后选择"C/C++: Reload Window"

3. 我还额外创建了以下配置文件来改善开发体验： - `.vscode/tasks.json`：定义了构建和运行测试的任务
   - `.vscode/launch.json`：定义了调试配置

完成这些步骤后，VSCode应该能够正确识别gtest/gtest.h头文件，错误提示应该消失。
