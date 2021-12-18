#include "player.hpp"

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>

#include "board.hpp"


namespace reviser::game {

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
} // namespace reviser::game
