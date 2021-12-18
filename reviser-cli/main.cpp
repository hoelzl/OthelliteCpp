// Copyright (c) 2021 Dr. Matthias Hölzl.

#include <iostream>

#include "board.hpp"
#include "console_notifier.hpp"
#include "default_game.hpp"
#include "direction.hpp"
#include "game.hpp"

using namespace reviser;
using namespace reviser::grid;
using namespace reviser::game;

int main()
{
    Direction constexpr d{0_N, 0_E};
    std::cout << "Direction is " << d << ".\n";
    std::cout << "The main compass directions are: " << N << ", " << E << ", " << S
              << ", " << W << ".\n";
    std::cout << "But we also have: " << NE << ", " << SE << ", " << SW << ", " << NW
              << ".\n";

    auto dark_player = RandomPlayer{"The dark player"};
    auto light_player = RandomPlayer{};
    auto const game = std::make_unique<DefaultGame<Board>>(
        dark_player, light_player, std::make_unique<ConsoleNotifier>());
    game->new_game(false);
    game->run_game_loop();
    game->new_game(true);
    game->run_game_loop();
    auto result = game->get_result();

    return 0;
}
