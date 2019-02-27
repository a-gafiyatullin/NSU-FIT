cmake_minimum_required(VERSION 3.13)
project(laboratory_work__1_2_ C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER /usr/bin/mpicc)

include_directories(/usr/include/mpi/)

add_executable(laboratory_work__1_2_ src/main.c src/matrix.c src/linear_equations.h src/linear_equations.c)

target_link_libraries(laboratory_work__1_2_ m)