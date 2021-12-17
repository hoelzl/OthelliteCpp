// Copyright (c) 2021 Dr. Matthias Hölzl.

#include <iostream>

#include "board.hpp"
#include "console_notifier.hpp"
#include "direction.hpp"
#include "game.hpp"
#include "game_impl.hpp"

using namespace othellite;
using namespace othellite::grid;
using namespace othellite::game;

int main()
{
    Direction constexpr d{0_N, 0_E};
    std::cout << "Direction is " << d << ".\n";
    std::cout << "The main compass directions are: " << N << ", " << E << ", " << S
              << ", " << W << ".\n";
    std::cout << "But we also have: " << NE << ", " << SE << ", " << SW << ", " << NW
              << ".\n";

    auto dark_player = RandomPlayer{};
    auto light_player = RandomPlayer{};
    auto game = std::make_unique<GameImpl<Board>>(
        dark_player, light_player, std::make_unique<ConsoleNotifier>());
    std::cout << game->get_board().to_string() << std::endl;
    return 0;
}
