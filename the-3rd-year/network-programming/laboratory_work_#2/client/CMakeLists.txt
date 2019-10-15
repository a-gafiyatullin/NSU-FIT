cmake_minimum_required(VERSION 3.15)
project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(client main.cpp Client.cpp Client.h)