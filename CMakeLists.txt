cmake_minimum_required(VERSION 3.28)
project(libdecimal)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.8.0
)

FetchContent_MakeAvailable(Catch2)

add_library(libdecimal
        src/decimal.cpp)

target_include_directories(libdecimal PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

add_executable(tests
        tests/unit_tests.cpp)

target_link_libraries(tests PRIVATE libdecimal Catch2::Catch2)

include(CTest)
enable_testing()

include(Catch)
catch_discover_tests(tests)