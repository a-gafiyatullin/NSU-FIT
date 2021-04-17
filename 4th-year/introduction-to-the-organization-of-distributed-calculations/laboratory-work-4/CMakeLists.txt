cmake_minimum_required(VERSION 3.5)

project(laboratory-work-4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(laboratory-work-4 main.cpp http_server.cpp http_server.h)
target_link_libraries(laboratory-work-4 pthread)

