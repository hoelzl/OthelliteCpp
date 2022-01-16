// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once

#ifndef REVISER_LIB_GAME_HPP
#define REVISER_LIB_GAME_HPP

#include <memory>

#include "game_result.hpp"
#include "player.hpp"

namespace reviser::game {

class Game
{
public:
    Game() = default;
    Game(const Game& other) = default;
    Game(Game&& other) noexcept = default;
    Game& operator=(const Game& other) = default;
    Game& operator=(Game&& other) noexcept = default;
    virtual ~Game() = default;

    virtual void new_game(bool swap_payers) = 0;
    virtual void run_game_loop() = 0;
    [[nodiscard]] virtual std::shared_ptr<const GameResult> get_result() const = 0;
};

class Players
{
public:
    Players(std::shared_ptr<Player> dp, std::shared_ptr<Player> lp)
        : dark_player{std::move(dp)}
        , light_player{std::move(lp)}
    {
        // Make sure not to access the arguments in the body of the constructor, since
        // we have moved from them!
        if (dark_player == nullptr || light_player == nullptr) {
            throw std::invalid_argument("Must provide valid players.");
        }
        dark_player->set_color(PlayerColor::dark);
        light_player->set_color(PlayerColor::light);
    }

    [[nodiscard]] const Player& get_dark_player() const { return *dark_player; }
    [[nodiscard]] Player& get_dark_player() { return *dark_player; }

    [[nodiscard]] const Player& get_light_player() const { return *light_player; }
    [[nodiscard]] Player& get_light_player() { return *light_player; }

    [[nodiscard]] const Player& get_other_player(const Player& player) const
    {
        if (player == get_light_player()) {
            return get_dark_player();
        }
        return get_light_player();
    }

    [[nodiscard]] Player& get_other_player(const Player& player)
    {
        if (player == get_light_player()) {
            return get_dark_player();
        }
        return get_light_player();
    }

    void swap_dark_and_light_player();
    void new_game();

private:
    std::shared_ptr<Player> dark_player;
    std::shared_ptr<Player> light_player;
};

class Notifier
{
public:
    Notifier() = default;
    Notifier(const Notifier& other) = default;
    Notifier(Notifier&& other) noexcept = default;
    Notifier& operator=(const Notifier& other) = default;
    Notifier& operator=(Notifier&& other) noexcept = default;
    virtual ~Notifier() = default;

    virtual void display_message(std::string_view message) = 0;
    virtual void display_board(const ArrayBoard& board);

    virtual void note_new_game(const Players& players, const ArrayBoard& board);

    virtual void
    note_move(const Player& player, grid::Position pos, const ArrayBoard& board);

    virtual void note_result(const GameResult& result);
};

} // namespace reviser::game

#endif
