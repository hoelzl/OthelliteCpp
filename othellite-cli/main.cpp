// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "direction.hpp"
#include <iostream>

using namespace othellite::grid;

int main()
{
    Direction d{0_N, 0_E};
    std::cout << "Direction is " << d << ".\n";
    std::cout << "The main compass directions are: " << N << ", " << E << ", " << S
              << ", " << W << ".\n";
    std::cout << "But we also have: " << NE << ", " << SE << ", " << SW << ", " << NW
              << ".\n";
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
