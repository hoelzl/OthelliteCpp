// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once
#ifndef REVISER_LIB_GAME_IMPL_HPP
#define REVISER_LIB_GAME_IMPL_HPP

#include <optional>

#include "board.hpp"
#include "game.hpp"
#include "game_result.hpp"

namespace reviser {

template <BoardType BoardT>
class DefaultGame final : public Game
{
public:
    DefaultGame(
        std::shared_ptr<Player> dark_player,
        std::shared_ptr<Player> light_player,
        std::unique_ptr<Notifier> notifier)
        : players{std::move(dark_player), std::move(light_player)}
        , notifier{std::move(notifier)}
        , board{std::make_unique<BoardT>()}
        , current_player{dark_player.get()}
    {}

    void new_game(bool swap_payers) override;
    void run_game_loop() override;
    [[nodiscard]] std::shared_ptr<const GameResult> get_result() const override;

    [[nodiscard]] const Players& get_players() const noexcept { return players; }
    [[nodiscard]] const BoardT& get_board() const noexcept { return *board; }
    [[nodiscard]] const Notifier& get_notifier() const noexcept { return *notifier; }

private:
    using Moves = std::set<Position>;

    Players players;

    std::unique_ptr<Notifier> notifier;
    std::unique_ptr<BoardT> board;
    Player* current_player;
    mutable std::optional<Moves> cached_moves_for_current_player{};
    mutable std::shared_ptr<GameResult> cached_result{};

    [[nodiscard]] Players& get_players() { return players; }
    [[nodiscard]] BoardT& get_board() noexcept { return *board; }
    [[nodiscard]] Notifier& get_notifier() noexcept { return *notifier; }

    [[nodiscard]] const Player& get_dark_player() const;
    [[nodiscard]] Player& get_dark_player();

    [[nodiscard]] const Player& get_light_player() const;
    [[nodiscard]] Player& get_light_player();

    [[nodiscard]] const Player& get_non_current_player() const;
    [[nodiscard]] Player& get_non_current_player();

    [[nodiscard]] const Player& get_current_player() const { return *current_player; }
    [[nodiscard]] Player& get_current_player() { return *current_player; }

    void set_current_player(Player& player);

    const Moves& get_valid_moves_for_current_player();
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
    cached_result = nullptr;
    get_board().initialize();
    if (swap_payers) {
        get_players().swap_dark_and_light_player();
    }
    set_current_player(get_dark_player());
    get_players().new_game();
    get_notifier().note_new_game(get_players(), *board.get());
}

template <BoardType BoardT>
void DefaultGame<BoardT>::run_game_loop()
{
    while (!cached_result) {
        pick_current_player_with_valid_moves();
        if (current_player_has_valid_moves()) {
            allow_current_player_to_move();
            swap_current_player();
        }
        else {
            set_result_from_score();
        }
    }
    get_notifier().note_result(*get_result());
}

template <BoardType BoardT>
std::shared_ptr<const GameResult> DefaultGame<BoardT>::get_result() const
{
    return cached_result;
}

template <BoardType BoardT>
const Player& DefaultGame<BoardT>::get_dark_player() const {
    return get_players().get_dark_player();
}

template <BoardType BoardT>
Player& DefaultGame<BoardT>::get_dark_player() { return get_players().get_dark_player(); }

template <BoardType BoardT>
const Player& DefaultGame<BoardT>::get_light_player() const {
    return get_players().get_light_player();
}

template <BoardType BoardT>
Player& DefaultGame<BoardT>::get_light_player() {
    return get_players().get_light_player();
}

template <BoardType BoardT>
const Player& DefaultGame<BoardT>::get_non_current_player() const {
    return get_players().get_other_player(get_current_player());
}

template <BoardType BoardT>
Player& DefaultGame<BoardT>::get_non_current_player() {
    return get_players().get_other_player(get_current_player());
}

template <BoardType BoardT>
void DefaultGame<BoardT>::set_current_player(Player& player)
{
    assert(player == get_dark_player() || player == get_light_player());
    current_player = &player;
    cached_moves_for_current_player = std::nullopt;
}

template <BoardType BoardT>
const typename DefaultGame<BoardT>::Moves&
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
    set_current_player(get_non_current_player());
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
        get_notifier().note_move(player, move, get_board());
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
        cached_result = std::make_unique<TiedResult>(
            score, get_board(), get_dark_player(), get_light_player());
    }
    else {
        const auto& [winner, loser] = score.compute_winner(get_players());
        cached_result = std::make_unique<WinByScore>(score, get_board(), winner, loser);
    }
}

template <BoardType BoardT>
void DefaultGame<BoardT>::disqualify_current_player()
{
    auto score = get_board().compute_score();

    cached_result = std::make_unique<WinByOpponentMistake>(
        score,
        get_board(),
        get_players().get_other_player(get_current_player()),
        get_current_player());
}

} // namespace reviser

#endif
