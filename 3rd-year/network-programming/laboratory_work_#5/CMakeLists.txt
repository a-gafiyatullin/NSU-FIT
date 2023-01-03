cmake_minimum_required(VERSION 3.15)
project(laboratory_work__29)

set(CMAKE_CXX_STANDARD 20)

add_executable(laboratory_work__29 src/client_processing/ClientProcessing.cpp
        src/client_processing/ClientProcessing.h src/server/Server.cpp src/server/Server.h
        src/main.cpp
        src/utility/SocksRequestParser.cpp
        src/utility/SocksRequestParser.h
        src/utility/network_utility.cpp
        src/utility/network_utility.h)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(laboratory_work__29 PUBLIC -DDEBUG)
endif ()

target_link_libraries(laboratory_work__29 -lcares)