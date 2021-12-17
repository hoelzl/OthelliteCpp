#pragma once

#ifndef OTHELLITE_LIB_GAME_IMPL_HPP
#define OTHELLITE_LIB_GAME_IMPL_HPP

#include <concepts>

#include "board.hpp"
#include "game.hpp"
#include "game_result.hpp"

namespace othellite::game {

template <typename BoardT>
requires std::derived_from<BoardT, Board>
class GameImpl : public Game
{
public:
    GameImpl(
        Player const& dark_player,
        Player const& light_player,
        std::unique_ptr<Notifier> notifier)
        : players{dark_player, light_player}
        , notifier{std::move(notifier)}
        , board{std::make_unique<BoardT>()}
    {}

    virtual void new_game(bool swap_payers) override;
    virtual void run_game_loop() override;
    [[nodiscard]] virtual std::unique_ptr<GameResult> get_result() const override;

    [[nodiscard]] Players const& get_players() const { return players; }
    [[nodiscard]] BoardT const& get_board() const { return *board; }

private:
    Players players;
    std::unique_ptr<Notifier> notifier;
    std::unique_ptr<BoardT> board;
};

template <typename BoardT>
requires std::derived_from<BoardT, Board>
void GameImpl<BoardT>::new_game(bool swap_payers) {}

template <typename BoardT>
requires std::derived_from<BoardT, Board>
void GameImpl<BoardT>::run_game_loop() {}


template <typename BoardT>
requires std::derived_from<BoardT, Board> //
    std::unique_ptr<GameResult> GameImpl<BoardT>::get_result()
const
{
    auto score = board->compute_score();
    auto const& [dark_player, light_player] = players;
    return std::make_unique<WinByScore>(score, *board, dark_player, light_player);
}

} // namespace othellite::game

#endif