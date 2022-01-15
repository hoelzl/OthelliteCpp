// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once

#ifndef REVISER_LIB_GAME_IMPL_HPP
#define REVISER_LIB_GAME_IMPL_HPP

#include <optional>

#include "array_board.hpp"
#include "game.hpp"
#include "game_result.hpp"

namespace reviser::game {

template <BoardType BoardT>
class DefaultGame final : public Game
{
public:
    DefaultGame(
        Player& dark_player, Player& light_player, std::unique_ptr<Notifier> notifier)
        : players{std::ref(dark_player), std::ref(light_player)}
        , notifier{std::move(notifier)}
        , board{std::make_unique<BoardT>()}
        , current_player{std::ref(dark_player)}
    {}

    void new_game(bool swap_payers) override;
    void run_game_loop() override;
    [[nodiscard]] std::shared_ptr<const GameResult> get_result() const override;

    [[maybe_unused]] [[nodiscard]] const Players& get_players() const
    {
        return players;
    }
    [[nodiscard]] BoardT& get_board() { return *board; }

private:
    Players players;
    std::unique_ptr<Notifier> notifier;
    std::unique_ptr<BoardT> board;
    std::reference_wrapper<Player> current_player;
    mutable std::optional<std::set<grid::Position>> cached_moves_for_current_player{};
    mutable std::shared_ptr<GameResult> result{};

    [[nodiscard]] Player& get_current_player() const;
    void set_current_player(Player& player);
    const std::set<grid::Position>& get_valid_moves_for_current_player();
    bool current_player_has_valid_moves();
    void swap_current_player();
    void pick_current_player_with_valid_moves();
    void allow_current_player_to_move();
    void set_result_from_score();
    void disqualify_current_player();
};

template <BoardType BoardT>
void DefaultGame<BoardT>::new_game(const bool swap_payers)
{
    result = nullptr;
    get_board().initialize();
    if (swap_payers) {
        players.swap_dark_and_light_player();
    }
    set_current_player(players.get_dark_player());
    players.new_game();
    notifier->note_new_game(players, *board.get());
}

template <BoardType BoardT>
void DefaultGame<BoardT>::run_game_loop()
{
    while (!result) {
        pick_current_player_with_valid_moves();
        if (current_player_has_valid_moves()) {
            allow_current_player_to_move();
            swap_current_player();
        }
        else {
            set_result_from_score();
        }
    }
    notifier->note_result(*get_result());
}

template <BoardType BoardT>
std::shared_ptr<const GameResult> DefaultGame<BoardT>::get_result() const
{
    return result;
}

template <BoardType BoardT>
Player& DefaultGame<BoardT>::get_current_player() const
{
    return current_player;
}

template <BoardType BoardT>
void DefaultGame<BoardT>::set_current_player(Player& player)
{
    assert(player == players.get_dark_player() || player == players.get_light_player());
    current_player = player;
    cached_moves_for_current_player = std::nullopt;
}

template <BoardType BoardT>
const std::set<grid::Position>&
DefaultGame<BoardT>::get_valid_moves_for_current_player()
{
    if (!cached_moves_for_current_player.has_value()) {
        cached_moves_for_current_player
            = get_board().find_valid_moves(get_current_player().get_color());
    }
    return cached_moves_for_current_player.value();
}

template <BoardType BoardT>
bool DefaultGame<BoardT>::current_player_has_valid_moves()
{
    return !get_valid_moves_for_current_player().empty();
}

template <BoardType BoardT>
void DefaultGame<BoardT>::swap_current_player()
{
    set_current_player(players.get_other_player(get_current_player()));
}

template <BoardType BoardT>
void DefaultGame<BoardT>::pick_current_player_with_valid_moves()
{
    // Sets the current player to a player with valid moves, if such a player exists.
    if (!current_player_has_valid_moves()) {
        swap_current_player();
    }
}

template <BoardType BoardT>
void DefaultGame<BoardT>::allow_current_player_to_move()
{
    auto& player = get_current_player();
    if (const auto move = player.pick_move(get_board());
        get_valid_moves_for_current_player().contains(move)) {
        get_board().play_move(player.get_color(), move);
        notifier->note_move(player, move, get_board());
    }
    else {
        disqualify_current_player();
    }
}

template <BoardType BoardT>
void DefaultGame<BoardT>::set_result_from_score()
{
    auto score = get_board().compute_score();

    if (score.is_tied()) {
        result = std::make_unique<TiedResult>(
            score, get_board(), players.get_dark_player(), players.get_light_player());
    }
    else {
        const auto& [winner, loser] = score.compute_winner(players);
        result = std::make_unique<WinByScore>(score, get_board(), winner, loser);
    }
}

template <BoardType BoardT>
void DefaultGame<BoardT>::disqualify_current_player()
{
    auto score = get_board().compute_score();

    result = std::make_unique<WinByOpponentMistake>(
        score,
        get_board(),
        players.get_other_player(get_current_player()),
        get_current_player());
}

} // namespace reviser::game

#endif
