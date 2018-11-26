cmake_minimum_required(VERSION 3.12)
project(with_vectorization C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(with_vectorization
        matrix.h
        test.c
        with_vectorization.c)
