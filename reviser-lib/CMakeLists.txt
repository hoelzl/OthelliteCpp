cmake_minimum_required(VERSION 3.21)
project(reviser)

add_library(reviser-lib
        "src/array_board.cpp"
        "include/array_board.hpp"
        "src/board.cpp"
        "include/board.hpp"
        "src/common.cpp"
        "include/common.hpp"
        "include/default_game.hpp"
        "src/direction.cpp"
        "include/direction.hpp"
        "src/game.cpp"
        "include/game.hpp"
        "src/game_result.cpp"
        "include/game_result.hpp"
        "src/player.cpp"
        "include/player.hpp"
        "src/position.cpp"
        "include/position.hpp"
)
target_include_directories(reviser-lib PUBLIC include)
