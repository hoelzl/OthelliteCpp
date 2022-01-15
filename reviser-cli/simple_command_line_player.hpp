// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once
#ifndef SIMPLE_COMMAND_LINE_PLAYER_HPP
#define SIMPLE_COMMAND_LINE_PLAYER_HPP

#include "player.hpp"

namespace reviser_cli {

using reviser::Board;
using reviser::grid::Position;

class SimpleCommandLinePlayer final : public reviser::game::Player
{
public:
    using Player::Player;

    [[nodiscard]] Position pick_move(const Board& board) const override;

private:
    [[nodiscard]] std::vector<Position>
    compute_possible_moves(const Board& board) const;

    void print_possible_moves(const std::vector<Position>& moves) const;
    void print_possible_move(const std::vector<Position>& moves, unsigned i) const;

    [[nodiscard]] static Position handle_user_input(const std::vector<Position>& moves);
};

} // namespace reviser_cli

#endif // SIMPLE_COMMAND_LINE_PLAYER_HPP