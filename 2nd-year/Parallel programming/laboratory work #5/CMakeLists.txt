cmake_minimum_required(VERSION 3.14)
project(laboratory_work__5 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER /usr/bin/mpicc)
set(CMAKE_C_FLAGS -pthread)

include_directories(/usr/include/mpi/)

add_executable(laboratory_work__5 src/main.c src/task.h src/task.c src/queue.c src/queue.h src/scheduler.h src/scheduler.c)
target_link_libraries(laboratory_work__5 m)