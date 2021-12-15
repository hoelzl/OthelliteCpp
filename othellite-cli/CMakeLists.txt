cmake_minimum_required(VERSION 3.21)
project(othellite-cli)

set(CMAKE_CXX_STANDARD 20)

add_executable(othellite-cli main.cpp)
target_link_libraries(othellite-cli othellite-lib)
