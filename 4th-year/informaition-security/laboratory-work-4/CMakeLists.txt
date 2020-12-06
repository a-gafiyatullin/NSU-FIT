cmake_minimum_required(VERSION 3.17)
project(laboratory_work_4 C)

set(CMAKE_C_STANDARD 11)

add_executable(laboratory_work_4 main.c wave.c wave.h)
target_link_libraries(laboratory_work_4 m)
