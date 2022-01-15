cmake_minimum_required(VERSION 3.21)
project(reviser)

set(CMAKE_CXX_STANDARD 23)

add_executable(reviser-test
        board_test.cpp
        common_test.cpp
        direction_test.cpp
        game_test.cpp
        position_test.cpp
        test_main.cpp
        utilities.hpp
        )
target_link_libraries(reviser-test PUBLIC reviser-lib)
target_include_directories(reviser-test PRIVATE external)
