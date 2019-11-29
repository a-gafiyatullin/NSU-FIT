cmake_minimum_required(VERSION 3.15)
project(laboratory_work__4)

set(CMAKE_CXX_STANDARD 20)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(laboratory_work__4 src/main.cpp src/utility/Point.cpp src/utility/Point.h src/frontend/Snake.cpp src/frontend/Snake.h src/frontend/GameField.cpp src/frontend/GameField.h)
target_link_libraries(laboratory_work__4 ${CURSES_LIBRARIES})
