// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "array_board.hpp"
#include "console_notifier.hpp"
#include "default_game.hpp"
#include "game.hpp"
#include "random_player.hpp"
#include "simple_command_line_player.hpp"

using reviser::ai::RandomPlayer;
using reviser::ArrayBoard;
using reviser::game::DefaultGame;
using reviser::game::Game;
using reviser_cli::ConsoleNotifier;
using reviser_cli::SimpleCommandLinePlayer;

int main()
{
    try {
        auto dark_player = std::make_shared<RandomPlayer>("The computer player");
        // auto light_player = std::make_shared<SimpleCommandLinePlayer>{"The human"};
        auto light_player = std::make_shared<RandomPlayer>("Not nearly human");
        const std::unique_ptr<Game> game = std::make_unique<DefaultGame<ArrayBoard>>(
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
