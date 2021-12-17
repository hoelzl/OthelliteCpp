#pragma once

#ifndef OTHELLITE_LIB_GAME_HPP
#define OTHELLITE_LIB_GAME_HPP

#include <cassert>
#include <functional>
#include <memory>

#include "game_result.hpp"
#include "player.hpp"

namespace othellite::game {

class Players
{
public:
    Players(Player& dark_player, Player& light_player)
        : dark_player{std::ref(dark_player)}
        , light_player{std::ref(light_player)}
    {
        dark_player.set_color(PlayerColor::dark);
		light_player.set_color(PlayerColor::light);
    }

    [[nodiscard]] Player& get_dark_player() const
    {
        return dark_player;
    }

    [[nodiscard]] Player& get_light_player() const
    {
        return light_player;
    }

	[[nodiscard]] Player& get_other_player(Player const& player) const
    {
		if (player == get_light_player()) {
		    return get_dark_player();
		} else {
		    return get_light_player();
		}
    }

    void swap_dark_and_light_player();
	void new_game() const;

private:
    std::reference_wrapper<Player> dark_player;
    std::reference_wrapper<Player> light_player;
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
    virtual void
    note_move(Player const& player, grid::Position pos, Board const& board);
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
    [[nodiscard]] virtual std::shared_ptr<GameResult const> get_result() const = 0;
};

} // namespace othellite::game

#endif