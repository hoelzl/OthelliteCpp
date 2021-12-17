#pragma once

#ifndef OTHELLITE_LIB_GAME_HPP
#define OTHELLITE_LIB_GAME_HPP

#include <memory>

#include "game_result.hpp"
#include "player.hpp"

namespace othellite::game {

struct Players
{
    Players(Player const& dark_player, Player const& light_player)
        : dark_player{dark_player}
        , light_player{light_player}
    {}

    Player const& dark_player;
    Player const& light_player;
};

class Notifier
{
public:
	Notifier() = default;
    Notifier(Notifier const& other) = default;
    Notifier(Notifier&& other) noexcept = default;
    Notifier& operator=(Notifier const& other) = default;
    Notifier& operator=(Notifier&& other) noexcept = default;
    virtual ~Notifier() = default;

    virtual void display_message(std::string_view message) = 0;
	virtual void display_board(Board const& board);

    virtual void note_new_game(Players const& players, Board board);
	virtual void note_move(Player const& player, grid::Position pos, Board const& board);
	virtual void note_result(GameResult const& result);
};

class Game
{
public:
    Game() = default;
    Game(Game const& other) = default;
    Game(Game&& other) noexcept = default;
    Game& operator=(Game const& other) = default;
    Game& operator=(Game&& other) noexcept = default;
    virtual ~Game() = default;

    virtual void new_game(bool swap_payers = false) = 0;
    virtual void run_game_loop() = 0;
    [[nodiscard]] virtual std::unique_ptr<GameResult> get_result() const = 0;
};

} // namespace othellite::game

#endif