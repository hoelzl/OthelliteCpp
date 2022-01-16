cmake_minimum_required(VERSION 3.21)
project(reviser-ai)

add_library(reviser-ai
    "src/random_player.cpp"
    "include/random_player.hpp"
)

target_link_libraries(reviser-ai reviser-lib)
target_include_directories(reviser-ai PUBLIC include)
