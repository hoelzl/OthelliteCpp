// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include <iostream>

#include "board.hpp"
#include "console_notifier.hpp"
#include "default_game.hpp"
#include "game.hpp"
#include "simple_command_line_player.hpp"

using reviser::Board;
using reviser::game::DefaultGame;
using reviser::game::RandomPlayer;
using reviser_cli::ConsoleNotifier;
using reviser_cli::SimpleCommandLinePlayer;

int main()
{
    try {
        auto dark_player = RandomPlayer{"The computer player"};
        auto light_player = SimpleCommandLinePlayer{"The human"};
        const auto game = std::make_unique<DefaultGame<Board>>(
            dark_player, light_player, std::make_unique<ConsoleNotifier>());
        game->new_game(false);
        game->run_game_loop();
        game->new_game(true);
        game->run_game_loop();
        auto result = game->get_result();
    }
    catch (const std::exception& ex) {
        std::printf("An error occurred: %s", ex.what());
    }
    return 0;
}
