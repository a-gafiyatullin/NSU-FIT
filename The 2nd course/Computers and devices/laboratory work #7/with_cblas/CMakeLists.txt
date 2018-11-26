cmake_minimum_required(VERSION 3.12)
project(with_cblas C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

include_directories(${BLAS_INCLUDE_DIR})

find_package(LAPACK REQUIRED)

find_package(BLAS REQUIRED)

add_executable(with_cblas
        cblas.c)

target_link_libraries(${CMAKE_PROJECT_NAME} ${BLAS_LIBRARIES} ${LAPACK_LIBRARIES})
