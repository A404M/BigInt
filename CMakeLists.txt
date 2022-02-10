cmake_minimum_required(VERSION 3.21)
project(BigInt)

set(CMAKE_CXX_COMPILER g++-10)

set(CMAKE_CXX_STANDARD 20)

add_executable(BigInt main.cpp big-int/BigInt.cpp big-int/BigInt.h)
target_link_libraries(BigInt gmp gmpxx)
