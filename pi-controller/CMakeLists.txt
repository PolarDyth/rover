cmake_minimum_required(VERSION 3.16)
project(pi_controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pi_controller
    src/main.cpp
    src/SerialPort.cpp
)
