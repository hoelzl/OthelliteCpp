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

[[maybe_unused]] void Player::game_over(const GameResult& result) {}

bool operator==(const Player& lhs, const Player& rhs) { return &lhs == &rhs; }

bool operator!=(const Player& lhs, const Player& rhs) { return !(lhs == rhs); }

auto RandomPlayer::pick_move(const Board& board) const -> grid::Position
{
    const auto moves = board.find_valid_moves(get_color());

    std::vector<grid::Position> out{};
    auto gen = std::mt19937{std::random_device{}()};
    std::ranges::sample(moves, std::back_inserter(out), 1, gen);

    assert(!out.empty());
    return out[0];
}

} // namespace reviser::game
