#include "simple_command_line_player.hpp"

#include <algorithm>
#include <format>

#include "position.hpp"
#include "board.hpp"

namespace reviser_cli {

Position SimpleCommandLinePlayer::pick_move(const Board& board) const
{
    const auto moves = compute_possible_moves(board);
    print_possible_moves(moves);
    return handle_user_input(moves);
}

std::vector<Position>
SimpleCommandLinePlayer::compute_possible_moves(const Board& board) const
{
    auto move_set = board.find_valid_moves(get_color());
    auto moves = std::vector<Position>{};
    moves.insert(end(moves), begin(move_set), end(move_set));
    std::ranges::sort(moves);
    return moves;
}

void SimpleCommandLinePlayer::print_possible_move(const std::vector<Position>& moves, unsigned i) const
{
    std::cout << std::format(
            "{:>4}: {:>2}, {:>2}", i, moves[i].get_row() + 1, moves[i].get_column() + 1)
        << "\n";
}

void SimpleCommandLinePlayer::print_possible_moves(const std::vector<Position>& moves) const
{
    std::cout << "\nYour possible moves are:\n";
    for (auto i = 0u; i < moves.size(); ++i) {
        print_possible_move(moves, i);
    }
    std::cout << "Please select a move: " << std::endl;
}

Position
SimpleCommandLinePlayer::handle_user_input(const std::vector<Position>& moves)
{
    size_t selection{};
    std::cin >> selection;
    if (selection < moves.size()) {
        return moves[selection];
    }
    else {
        std::cout << "Invalid input, returning first move.\n";
        return moves[0];
    }
}

}
