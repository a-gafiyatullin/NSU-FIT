cmake_minimum_required(VERSION 3.13)
project(laboratory_work__3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER /usr/bin/mpicc)

include_directories(/usr/include/mpi/)

add_executable(laboratory_work__3 main.c matrix.h matrix.c MPI_mul_matrices.h MPI_mul_matrices.c)