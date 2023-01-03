cmake_minimum_required(VERSION 3.14)
project(laboratory_work__4 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER /usr/bin/mpicc)

include_directories(/usr/include/mpi/)

add_executable(laboratory_work__4 src/main.c src/jacobi_method.h src/jacobi_method.c)