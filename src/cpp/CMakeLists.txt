cmake_minimum_required(VERSION 3.26)
project(tftp_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(tftp_cpp main.cpp
        client.cpp
        filepaths.cpp
        logger.cpp
        myTimer.cpp)

#-lws2_32
target_link_libraries(tftp_cpp wsock32 ws2_32)


