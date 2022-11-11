cmake_minimum_required(VERSION 3.21)
project(LSB_injection)

set(CMAKE_CXX_STANDARD 14)

add_executable(LSB_injection main.cpp LSB.cpp LSB.h)
