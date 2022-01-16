// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once

#ifndef TEST_UTILITIES_HPP
#define TEST_UTILITIES_HPP

#include <algorithm>
#include <sstream>
#include <utility>

#include "array_board.hpp"
#include "game.hpp"
#include "player.hpp"
#include "position.hpp"

namespace reviser {
using ::std::ranges::min;

struct ConstantPlayerStub final : public Player
{
    explicit ConstantPlayerStub(
        const std::string_view player_name = "Unnamed Player",
        const PlayerColor pc = PlayerColor::dark,
        Position played_position = Position{Row{0}, Column{0}})
        : played_position{played_position}
    {
        set_name(player_name);
        set_color(pc);
    }

    [[nodiscard]] Position
    pick_move(const ArrayBoard& board) const override
    {
        return played_position;
    }

    Position played_position;
};

struct MinimalPlayer final : public Player
{
    using Player::Player;

    [[nodiscard]] Position
    pick_move(const ArrayBoard& board) const override
    {
        auto moves = board.find_valid_moves(get_color());
        return min(moves);
    }
};

struct SpyForNotifierOutput final : public Notifier
{
    void display_message(const std::string_view message) override
    {
        stream << message << "\n";
    }

    [[nodiscard]] std::string output() const { return stream.str(); }

    std::stringstream stream{};
};

struct SpyForNotifierMoves final : public Notifier
{
    void display_message(std::string_view message) override
    {
        messages.emplace_back(message);
    }

    void display_board(const ArrayBoard& board) override
    {
        boards.push_back(board);
    }

    void
    note_new_game(const Players& players, const ArrayBoard& board) override
    {}

    void note_move(
        const Player& player,
        Position pos,
        const ArrayBoard& board) override
    {
        moves.emplace_back(
            player.get_name().data(),
            player.get_color(),
            pos.get_row(),
            pos.get_column());
    }

    void note_result(const GameResult& result) override
    {
        if (auto* win_result = dynamic_cast<const WinByScore*>(&result)) {
            result_summary.type = "win";
            result_summary.winner = win_result->get_winner().get_color();
            result_summary.loser = win_result->get_loser().get_color();
        }
        else if (auto* tie_result = dynamic_cast<const TiedResult*>(&result)) {
            result_summary.type = "tie";
            result_summary.winner = tie_result->get_dark_player().get_color();
            result_summary.loser = tie_result->get_light_player().get_color();
        }
        else if (
            auto* goof_result
            = dynamic_cast<const WinByOpponentMistake*>(&result)) {
            result_summary.type = "wrong_move";
            result_summary.winner = goof_result->get_winner().get_color();
            result_summary.loser = goof_result->get_loser().get_color();
        }
    }

    struct Move
    {
        Move(std::string player_name, PlayerColor color, int row, int column)
            : player_name(std::move(player_name))
            , color(color)
            , row(row)
            , column(column)
        {}
        std::string player_name{};
        PlayerColor color{};
        int row{};
        int column{};
    };

    struct Result
    {
        std::string type{"unknown"};
        PlayerColor winner{};
        PlayerColor loser{};
    };

    std::vector<std::string> messages{};
    std::vector<ArrayBoard> boards{};
    std::vector<Move> moves{};
    Result result_summary{};
};

inline bool
operator==(const SpyForNotifierMoves::Move& lhs, const SpyForNotifierMoves::Move& rhs)
{
    return lhs.player_name == rhs.player_name && lhs.color == rhs.color
           && lhs.row == rhs.row && lhs.column == rhs.column;
}

inline std::ostream& operator<<(std::ostream& os, const SpyForNotifierMoves& spy)
{
    for (auto& [player_name, color, row, column] : spy.moves) {
        os << "    {\"" << player_name
           << "\", PlayerColor::" << player_color_to_string(color) << ", " << row
           << ", " << column << "},\n";
    }
    os << "\n\nResult: " << spy.result_summary.type
       << ", winner: " << player_color_to_string(spy.result_summary.winner)
       << ", loser: " << player_color_to_string(spy.result_summary.loser) << "\n";

    return os;
}

} // namespace reviser

#endif
