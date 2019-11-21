cmake_minimum_required(VERSION 3.15)
project(laboratory_work__3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(laboratory_work__3 main.cpp Node.cpp Node.h IPv4Address.cpp IPv4Address.h)