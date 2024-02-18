cmake_minimum_required(VERSION 3.23)
project(SoftRasterizationRenderer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SoftRasterizationRenderer
        main.cpp
        tgaimage.cpp
        tgaimage.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")