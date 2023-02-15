// Copyright (c) 2021-2023 Dr. Matthias Hölzl.

#pragma once
#ifndef SIMPLE_COMMAND_LINE_PLAYER_HPP
#define SIMPLE_COMMAND_LINE_PLAYER_HPP

#include <vector>

#include "player.hpp"

namespace reviser_cli {

    class SimpleCommandLinePlayer final : public reviser::Player {
    public:
        using Player::Player;

        [[nodiscard]] reviser::Position
        pick_move(const reviser::BasicBoard &board) const override;

    private:
        [[nodiscard]] std::vector<reviser::Position>
        compute_possible_moves(const reviser::BasicBoard &board) const;

        void print_possible_moves(const std::vector<reviser::Position> &moves) const;

        void
        print_possible_move(const std::vector<reviser::Position> &moves, unsigned i) const;

        [[nodiscard]] static reviser::Position
        handle_user_input(const std::vector<reviser::Position> &moves);
    };

} // namespace reviser_cli

#endif // SIMPLE_COMMAND_LINE_PLAYER_HPP