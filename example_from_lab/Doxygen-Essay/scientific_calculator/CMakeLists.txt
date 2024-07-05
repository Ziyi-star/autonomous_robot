set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
cmake_minimum_required(VERSION 3.14)
project(scientific_calculator C)

set(CMAKE_C_STANDARD 99)

add_executable(scientific_calculator calculator.c math_library.c math_library.h)
target_link_libraries(scientific_calculator m)

