cmake_minimum_required(VERSION 3.21)
project(reviser)

add_library(reviser-lib
        "array_board.cpp"
        "array_board.hpp"
        "board.cpp"
        "board.hpp"
        "common.cpp"
        "common.hpp"
        "default_game.hpp"
        "direction.cpp"
        "direction.hpp"
        "game.cpp"
        "game.hpp"
        "game_result.cpp"
        "game_result.hpp"
        "player.cpp"
        "player.hpp"
        "position.cpp"
        "position.hpp"
)
target_include_directories(reviser-lib PUBLIC .)
