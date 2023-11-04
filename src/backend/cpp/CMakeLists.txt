# 指定CMake的最低版本要求
cmake_minimum_required(VERSION 3.0)

# 定义项目名称
project(myTftpClient)

# 添加可执行文件
add_executable(main main.cpp logger.cpp client.cpp
        client.cpp
        filepaths.cpp
        myTimer.cpp
        myTimer.h
        oat_main.cpp
        handler.h)

# 如果需要链接额外的库（例如，Windows套接字库）
target_link_libraries(main wsock32)
