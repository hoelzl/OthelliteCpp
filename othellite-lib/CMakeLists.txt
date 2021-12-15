cmake_minimum_required(VERSION 3.21)
project(othellite)

set(CMAKE_CXX_STANDARD 20)

add_library(othellite-lib
        board.cpp
        board.hpp
        direction.cpp
        direction.hpp
        common.hpp
        position.cpp
        position.hpp
        common.cpp)
target_include_directories(othellite-lib PUBLIC .)
