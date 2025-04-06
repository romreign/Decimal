cmake_minimum_required(VERSION 3.28)
project(libdecimal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_library(decimal src/decimal.cpp)
target_include_directories(decimal PUBLIC include)
add_executable(unit_tests tests/unit_tests.cpp)

target_link_libraries(unit_tests decimal)

find_package(Catch2 REQUIRED)
target_link_libraries(unit_tests Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(unit_tests)



