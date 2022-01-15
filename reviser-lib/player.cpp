// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "player.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <vector>

#include "array_board.hpp"


namespace reviser::game {

void Player::new_game() {}

[[maybe_unused]] void Player::game_over(const GameResult& result) {}

bool operator==(const Player& lhs, const Player& rhs) { return &lhs == &rhs; }

bool operator!=(const Player& lhs, const Player& rhs) { return !(lhs == rhs); }

auto RandomPlayer::pick_move(const ArrayBoard& board) const -> grid::Position
{
    const auto moves = board.find_valid_moves(get_color());

    std::vector<grid::Position> out{};
    std::ranges::sample(moves, std::back_inserter(out), 1, make_rng());

    assert(!out.empty());
    return out[0];
}

} // namespace reviser::game
