cmake_minimum_required(VERSION 3.15)
project(laboratory_work__2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(laboratory_work__2 main.cpp ClientProcessing.h ClientProcessing.cpp Server.cpp Server.h)