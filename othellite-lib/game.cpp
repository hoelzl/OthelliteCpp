
#include "game.hpp"

#include <format>
#include <string>

#include "board.hpp"
#include "game_result.hpp"


namespace othellite::game {

using namespace std::string_literals;

void Notifier::display_board(Board const& board) { display_message(board.to_string()); }

void Notifier::note_new_game(Players const& players, Board board)
{
    auto const& [dark_player, light_player] = players;
    display_message("Starting a new game.");

    auto message = std::format("Dark player: {}", dark_player.get().get_name());
    display_message(message);

    message = std::format("Light player: {}", light_player.get().get_name());
    display_message(message);
}

void Notifier::note_move(
    Player const& player, grid::Position const pos, Board const& board)
{
    auto const message = std::format(
        "Player ({}) plays ({}, {}).",
        player.get_name(),
        pos.get_row().value,
        pos.get_column().value);

    display_message(message);
    display_board(board);
}

void Notifier::note_result(GameResult const& result)
{
    auto const message = "GAME OVER.\n"s + result.to_string();
	display_message(message);
}

} // namespace othellite::game
