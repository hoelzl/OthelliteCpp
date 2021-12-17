cmake_minimum_required(VERSION 3.21)
project(othellite)

set(CMAKE_CXX_STANDARD 23)

add_library(othellite-lib
    board.cpp
    board.hpp
    common.cpp
    common.hpp
    console_notifier.cpp
    console_notifier.hpp
    direction.cpp
    direction.hpp
    game.cpp
    game.hpp
    game_impl.hpp
    game_result.cpp   
    game_result.hpp
    player.cpp
    player.hpp
    position.cpp
    position.hpp
 )
target_include_directories(othellite-lib PUBLIC .)
