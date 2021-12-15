cmake_minimum_required(VERSION 3.21)
project(othellite)

set(CMAKE_CXX_STANDARD 20)

add_executable(othellite-test
        board_test.cpp
        test_main.cpp
        direction_test.cpp
        position_test.cpp
        common_test.cpp)
target_link_libraries(othellite-test PUBLIC othellite-lib)
target_include_directories(othellite-test PRIVATE external)
