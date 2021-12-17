#pragma once

#ifndef OTHELLITE_LIB_GAME_RESULT_HPP
#define OTHELLITE_LIB_GAME_RESULT_HPP

#include <string>

#include "common.hpp"

namespace othellite {
class Board;
class Score;
}

namespace othellite::game {
class Player;

class GameResult
{
public:
    GameResult(Score score, Board const& board)
        : score{score}
        , board{&board}
    {}

    GameResult(GameResult const& other) = default;
    GameResult(GameResult&& other) noexcept = default;
    GameResult& operator=(GameResult const& other) = default;
    GameResult& operator=(GameResult&& other) noexcept = default;
    virtual ~GameResult() = default;

    [[nodiscard]] virtual std::string to_string() const = 0;

    [[nodiscard]] virtual Score get_score() const { return score; }
    [[nodiscard]] virtual Board const& get_board() const { return *board; }

private:
    Score score;
    Board const* board;
};

class DecisiveGameResult : public GameResult
{
public:
    DecisiveGameResult(
        Score const score,
        Board const& board,
        Player const& winner,
        Player const& loser)
        : GameResult{score, board}
        , winner{&winner}
        , loser{&loser}
    {}

    [[nodiscard]] Player const& get_winner() const { return *winner; }
    [[nodiscard]] Player const& get_loser() const { return *loser; }

    Player const* winner;
    Player const* loser;
};

class WinByScore final : public DecisiveGameResult
{
public:
    using DecisiveGameResult::DecisiveGameResult;
    [[nodiscard]] virtual std::string to_string() const override;
};

class WinByOpponentMistake final : public DecisiveGameResult
{
public:
    using DecisiveGameResult::DecisiveGameResult;
    [[nodiscard]] virtual std::string to_string() const override;
};


class TiedResult final : public GameResult
{
public:
    TiedResult(
        Score const score,
        Board const& board,
        Player const& dark_player,
        Player const& light_player)
        : GameResult{score, board}
        , dark_player{&dark_player}
        , light_player{&light_player}
    {}

    [[nodiscard]] Player const& get_dark_player() const { return *dark_player; }
    [[nodiscard]] Player const& get_light_player() const { return *light_player; }

    [[nodiscard]] virtual std::string to_string() const override;
private:
    Player const* dark_player;
    Player const* light_player;
};


} // namespace othellite::game
#endif // OTHELLITE_LIB_GAME_RESULT_HPP
