cmake_minimum_required(VERSION 3.21)
project(reviser-cli)

add_executable(reviser-cli
    "src/main.cpp"
    "src/console_notifier.cpp"
    "include/console_notifier.hpp"
    "src/simple_command_line_player.cpp"
    "include/simple_command_line_player.hpp")

target_link_libraries(reviser-cli reviser-lib reviser-ai)
target_include_directories(reviser-cli PUBLIC include)
