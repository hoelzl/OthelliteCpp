cmake_minimum_required(VERSION 3.21)
project(reviser-cli)

add_executable(reviser-cli
    "main.cpp"
    "console_notifier.cpp"
    "console_notifier.hpp"
    "simple_command_line_player.cpp"
    "simple_command_line_player.hpp")

target_link_libraries(reviser-cli reviser-lib reviser-ai)
