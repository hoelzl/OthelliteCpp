// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "player.hpp"

namespace reviser::game {

void Player::new_game() {}

[[maybe_unused]] void Player::game_over(const GameResult& result) {}

bool operator==(const Player& lhs, const Player& rhs) { return &lhs == &rhs; }

bool operator!=(const Player& lhs, const Player& rhs) { return !(lhs == rhs); }

} // namespace reviser::game
