// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "array_board.hpp"
#include "console_notifier.hpp"
#include "default_game.hpp"
#include "game.hpp"
#include "random_player.hpp"
#include "simple_command_line_player.hpp"

using reviser::ArrayBoard;
using reviser::DefaultGame;
using reviser::Game;
using reviser::Player;
using reviser::ai::RandomPlayer;
using reviser_cli::ConsoleNotifier;
using reviser_cli::SimpleCommandLinePlayer;

using namespace std::literals::string_literals;

enum class GameType
{
    human_vs_computer,
    computer_vs_computer,
};

int main(int argc, const char** argv)
{
    auto game_type{GameType::computer_vs_computer};
    if (argc >= 2) {
        if (const std::string arg_1{argv[1]}; arg_1 == "--human"s || arg_1 == "-h"s) {
            game_type = GameType::human_vs_computer;
        }
    }

    try {
        auto dark_player = std::make_shared<RandomPlayer>("The computer player");
        std::shared_ptr<Player> light_player;
        if (game_type == GameType::human_vs_computer) {
            light_player = std::make_shared<SimpleCommandLinePlayer>("The human");
        }
        else {
            light_player = std::make_shared<RandomPlayer>("Not nearly human");
        }
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
