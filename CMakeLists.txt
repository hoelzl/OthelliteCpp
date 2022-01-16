cmake_minimum_required(VERSION 3.21)
project(reviser-lib)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(reviser-ai)
add_subdirectory(reviser-cli)
add_subdirectory(reviser-lib)
add_subdirectory(test)
