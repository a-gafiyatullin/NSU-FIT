cmake_minimum_required(VERSION 3.13)
project(laboratory_work__1 C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/include/mpi/)

add_executable(laboratory_work__1 main.c matrix.c)