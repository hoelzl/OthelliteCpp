cmake_minimum_required(VERSION 3.21)
project(reviser-cli)

set(CMAKE_CXX_STANDARD 20)

add_executable(reviser-cli main.cpp)
target_link_libraries(reviser-cli reviser-lib)
