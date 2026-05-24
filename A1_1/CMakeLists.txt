cmake_minimum_required(VERSION 4.0)
project(A1_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(A1_1 StringGenerator.cpp
        StringSortTester.cpp
        main.cpp
        StringGenerator.h
        StringSortTester.h
)
