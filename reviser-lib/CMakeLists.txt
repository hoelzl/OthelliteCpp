cmake_minimum_required(VERSION 3.21)
project(reviser)

set(CMAKE_CXX_STANDARD 23)

add_library(reviser-lib
        array_board.cpp
        array_board.hpp
        board.hpp
        board.cpp
        common.cpp
        common.hpp
        direction.cpp
        direction.hpp
        game.cpp
        game.hpp
        default_game.hpp
        game_result.cpp
        game_result.hpp
        player.cpp
        player.hpp
        position.cpp
        position.hpp
        )
target_include_directories(reviser-lib PUBLIC .)
