cmake_minimum_required(VERSION 3.26)
project(MB)

set(CMAKE_CXX_STANDARD 17)

add_executable(MB main.cpp
        CFG.cpp
        CFG.h)
