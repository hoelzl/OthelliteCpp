#include "player.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <random>
#include <vector>

#include "board.hpp"


namespace reviser::game {

using ::std::ranges::sort;

void Player::new_game() {}

[[maybe_unused]] void Player::game_over(GameResult const& result) {}

bool operator==(Player const& lhs, Player const& rhs) { return &lhs == &rhs; }

bool operator!=(Player const& lhs, Player const& rhs) { return !(lhs == rhs); }

auto RandomPlayer::pick_move(Board const& board) const -> grid::Position
{
    auto const moves = board.find_valid_moves(get_color());

    std::vector<grid::Position> out{};
    auto gen = std::mt19937{std::random_device{}()};
    std::ranges::sample(moves, std::back_inserter(out), 1, gen);

    assert(!out.empty());
    return out[0];
}

grid::Position SimpleCommandLinePlayer::pick_move(Board const& board) const
{
    auto const moves = compute_possible_moves(board);
    print_possible_moves(moves);
    return handle_user_input(moves);
}

std::vector<grid::Position>
SimpleCommandLinePlayer::compute_possible_moves(Board const& board) const
{
    auto move_set = board.find_valid_moves(get_color());
    auto moves = std::vector<grid::Position>{};
    moves.insert(end(moves), begin(move_set), end(move_set));
    sort(moves);
    return moves;
}

void SimpleCommandLinePlayer::print_possible_moves(
    std::vector<grid::Position> const& moves) const
{
    std::cout << "\nYour possible moves are:\n";
    for (auto i = 0u; i < moves.size(); ++i) {
        std::cout << std::format(
            "{:>4}: {:>2}, {:>2}", i, moves[i].get_row() + 1, moves[i].get_column() + 1)
                  << "\n";
    }
    std::cout << "Please select a move: " << std::endl;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
grid::Position SimpleCommandLinePlayer::handle_user_input(
    std::vector<grid::Position> const& moves) const
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

} // namespace reviser::game
