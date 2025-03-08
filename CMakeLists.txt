cmake_minimum_required(VERSION 3.28)
project(libdecimal)

set(CMAKE_CXX_STANDARD 17)

add_executable(libdecimal
                    src/decimal.cpp
                    include/decimal.h)
