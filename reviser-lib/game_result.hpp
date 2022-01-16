// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once

#ifndef REVISER_LIB_GAME_RESULT_HPP
#define REVISER_LIB_GAME_RESULT_HPP

#include <functional>
#include <string>

#include "common.hpp"

namespace reviser {
class Score;
} // namespace reviser

namespace reviser {
class ArrayBoard;
}

namespace reviser {

class Player;

class GameResult
{
public:
    GameResult(Score score, const ArrayBoard& board)
        : score{score}
        , board{std::ref(board)}
    {}

    GameResult(const GameResult& other) = default;
    GameResult(GameResult&& other) noexcept = default;
    GameResult& operator=(const GameResult& other) = default;
    GameResult& operator=(GameResult&& other) noexcept = default;
    virtual ~GameResult() = default;

    [[nodiscard]] virtual std::string to_string() const = 0;

    [[nodiscard]] virtual Score get_score() const { return score; }
    [[maybe_unused]] [[nodiscard]] virtual const ArrayBoard& get_board() const;

private:
    Score score;
    std::reference_wrapper<const ArrayBoard> board;
};

class DecisiveGameResult : public GameResult
{
public:
    DecisiveGameResult(
        const Score score,
        const ArrayBoard& board,
        const Player& winner,
        const Player& loser)
        : GameResult{score, board}
        , winner{std::ref(winner)}
        , loser{std::ref(loser)}
    {}

    [[nodiscard]] const Player& get_winner() const { return winner; }
    [[nodiscard]] const Player& get_loser() const { return loser; }

private:
    std::reference_wrapper<const Player> winner;
    std::reference_wrapper<const Player> loser;
};

class WinByScore final : public DecisiveGameResult
{
public:
    using DecisiveGameResult::DecisiveGameResult;
    [[nodiscard]] std::string to_string() const override;
};

class WinByOpponentMistake final : public DecisiveGameResult
{
public:
    using DecisiveGameResult::DecisiveGameResult;
    [[nodiscard]] std::string to_string() const override;
};


class TiedResult final : public GameResult
{
public:
    TiedResult(
        const Score score,
        const ArrayBoard& board,
        const Player& dark_player,
        const Player& light_player)
        : GameResult{score, board}
        , dark_player{std::ref(dark_player)}
        , light_player{std::ref(light_player)}
    {}

    [[nodiscard]] const Player& get_dark_player() const { return dark_player; }
    [[nodiscard]] const Player& get_light_player() const { return light_player; }

    [[nodiscard]] std::string to_string() const override;

private:
    std::reference_wrapper<const Player> dark_player;
    std::reference_wrapper<const Player> light_player;
};


} // namespace reviser
#endif // REVISER_LIB_GAME_RESULT_HPP
