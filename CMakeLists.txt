cmake_minimum_required(VERSION 3.17)
project(CHIP8)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
set (CMAKE_CXX_FLAGS "-lSDL2")
include_directories(. ${SDL2_INCLUDE_DIRS})

add_executable(CHIP8
        chip8.cpp
        main.cpp)
target_link_libraries(CHIP8 ${SDL2_LIBRARIES})
