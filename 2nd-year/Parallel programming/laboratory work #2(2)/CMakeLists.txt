cmake_minimum_required(VERSION 3.13)
project(laboratory_work__2_2_ C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

add_executable(laboratory_work__2_2 src/main.c src/matrix.c src/linear_equations.h src/linear_equations.c)
target_link_libraries(laboratory_work__2_2 m)