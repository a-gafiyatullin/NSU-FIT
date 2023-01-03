cmake_minimum_required(VERSION 3.12)
project(laboratory_work__7 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(laboratory_work__7
        matrix.h
        test.c
        without_vectorization.c)
