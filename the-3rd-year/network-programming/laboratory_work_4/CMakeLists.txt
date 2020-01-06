cmake_minimum_required(VERSION 3.15)
project(laboratory_work__4)

set(CMAKE_CXX_STANDARD 20)
find_package(Curses REQUIRED)
include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CURSES_INCLUDE_DIRS})

protobuf_generate_cpp(PROTO_SRC PROTO_HEADER src/protobuf/snakes.proto)
add_library(proto ${PROTO_HEADER} ${PROTO_SRC})

add_executable(laboratory_work__4
        src/main.cpp
        src/backend/Coord.cpp
        src/backend/Coord.h
        src/backend/Snake.cpp
        src/backend/Snake.h
        src/frontend/GameField.cpp
        src/frontend/GameField.h
        src/backend/Game.h
        src/backend/Game.cpp
        src/protobuf/snakes.pb.cc
        src/protobuf/snakes.pb.h src/utility/IPv4Address.cpp src/utility/IPv4Address.h)

target_link_libraries(laboratory_work__4 proto ${PROTOBUF_LIBRARY} ${CURSES_LIBRARIES} -pthread -ltinyxml)
