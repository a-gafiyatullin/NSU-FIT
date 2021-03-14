cmake_minimum_required(VERSION 3.17)
project(laboratory_work__2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER /usr/bin/mpicxx)

include_directories(/usr/include/mpi/)

add_executable(laboratory_work__2 main.cpp)
