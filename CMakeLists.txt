# cmake_minimum_required(VERSION <specify CMake version here>)
project(task3)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-march=native")

add_executable(task3 main.cpp MyUint.h)