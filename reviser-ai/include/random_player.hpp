// Copyright (c) 2022-2023 Dr. Matthias Hölzl.

#pragma once
#ifndef RANDOM_PLAYER_HPP
#define RANDOM_PLAYER_HPP

#include <random>

#include "board.hpp"
#include "player.hpp"

namespace reviser::ai {

    class RandomPlayer final : public Player {
    public:
        using Player::Player;

        [[nodiscard]] Position pick_move(const BasicBoard &board) const override;

    private:
        static std::mt19937 make_rng() { return std::mt19937{std::random_device{}()}; }
    };

} // namespace reviser::ai

#endif // RANDOM_PLAYER_HPP