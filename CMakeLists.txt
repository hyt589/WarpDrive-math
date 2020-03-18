cmake_minimum_required(VERSION 3.0.0)
project(warp-math)

include_directories(include)
file(GLOB SOURCES "src/*.cpp" "main.cpp")

add_library(WarpMath SHARED ${SOURCES})

add_executable(test ${SOURCES})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")