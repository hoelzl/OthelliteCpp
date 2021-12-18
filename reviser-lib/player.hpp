#pragma once

#ifndef REVISER_LIB_PLAYER_HPP
#define REVISER_LIB_PLAYER_HPP

#include <string_view>

#include "common.hpp"
#include "position.hpp"

namespace reviser {
class Board;
}

namespace reviser::game {
class GameResult;


class Player
{
public:
    explicit Player(
        std::string_view const name = "An unnamed player",
        PlayerColor const pc = PlayerColor::dark)
        : name{name}
        , color{pc}
    {}
    Player(Player const&) = default;
    Player(Player&&) noexcept = default;
    Player& operator=(Player const&) = default;
    Player& operator=(Player&&) noexcept = default;
    virtual ~Player() = default;

    [[nodiscard]] std::string_view get_name() const { return name; }
    [[nodiscard]] PlayerColor get_color() const { return color; }
    void set_color(PlayerColor const pc) { this->color = pc; }

    virtual void new_game();
    [[nodiscard]] virtual grid::Position pick_move(Board const& board) const = 0;
    [[maybe_unused]] virtual void game_over(GameResult const& result);

protected:
    std::string name{"An unnamed player"};
    PlayerColor color{};
};


bool operator==(Player const& lhs, Player const& rhs);
bool operator!=(Player const& lhs, Player const& rhs);


class RandomPlayer final : public Player
{
public:
    using Player::Player;
    [[nodiscard]] grid::Position pick_move(Board const& board) const override;
};
} // namespace reviser::game

#endif
