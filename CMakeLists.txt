cmake_minimum_required(VERSION 3.23)
project(SoftRasterizationRenderer)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(SoftRasterizationRenderer
        main.cpp
        tgaimage.cpp
        tgaimage.h
        geometry.h
        model.cpp
        model.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")