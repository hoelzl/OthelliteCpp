#pragma once

#ifndef OTHELLITE_LIB_PLAYER_HPP
#define OTHELLITE_LIB_PLAYER_HPP

#include "common.hpp"
#include "position.hpp"

namespace othellite {
class Board;
}

namespace othellite::game {
class GameResult;

class Player
{
public:
    Player() = default;
    Player(Player const&) = default;
    Player(Player&&) noexcept = default;
    Player& operator=(Player const&) = default;
    Player& operator=(Player&&) noexcept = default;
    virtual ~Player() = default;

    [[nodiscard]] std::string_view get_name() const { return name; }
    [[nodiscard]] PlayerColor get_color() const { return color; }

    virtual void start_game(PlayerColor pc);
    [[nodiscard]] virtual grid::Position pick_move(Board const& board) const = 0;
    virtual void game_over(GameResult const& result);

protected:
    std::string name{};
    PlayerColor color{PlayerColor::light};
};

class RandomPlayer : public Player
{
public:
    [[nodiscard]] virtual grid::Position pick_move(Board const& board) const override;
};
} // namespace othellite::game

#endif
