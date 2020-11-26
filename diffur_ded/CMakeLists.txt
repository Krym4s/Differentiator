cmake_minimum_required(VERSION 3.17)
project(diffur_ded)

set(CMAKE_CXX_STANDARD 14)

add_executable(diffur_ded main.cpp cmake-build-debug/IsE_Tree.cpp cmake-build-debug/Numeric_Tree.cpp cmake-build-debug/Diffur.cpp cmake-build-debug/IsE_text.cpp)