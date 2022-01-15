#pragma once

#ifndef REVISER_LIB_PLAYER_HPP
#define REVISER_LIB_PLAYER_HPP

#include <vector>

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
        const std::string_view name = "An unnamed player",
        const PlayerColor pc = PlayerColor::dark)
        : name{name}
        , color{pc}
    {}

    Player(const Player&) = delete;
    Player(Player&&) noexcept = default;
    Player& operator=(const Player&) = delete;
    Player& operator=(Player&&) noexcept = default;
    virtual ~Player() = default;

    [[nodiscard]] const std::string& get_name() const { return name; }
    void set_name(std::string_view new_name) { this->name = new_name; }

    [[nodiscard]] PlayerColor get_color() const { return color; }
    void set_color(const PlayerColor pc) { this->color = pc; }

    virtual void new_game();
    [[nodiscard]] virtual grid::Position pick_move(const Board& board) const = 0;
    [[maybe_unused]] virtual void game_over(const GameResult& result);

private:
    std::string name{"An unnamed player"};
    PlayerColor color{};
};


bool operator==(const Player& lhs, const Player& rhs);
bool operator!=(const Player& lhs, const Player& rhs);


class RandomPlayer final : public Player
{
public:
    using Player::Player;
    [[nodiscard]] grid::Position pick_move(const Board& board) const override;
};

} // namespace reviser::game

#endif
