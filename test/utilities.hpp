// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once

#ifndef TEST_UTILITIES_HPP
#define TEST_UTILITIES_HPP

#include <algorithm>
#include <sstream>
#include <utility>

#include "board.hpp"
#include "game.hpp"
#include "player.hpp"
#include "position.hpp"

namespace reviser {
using std::ranges::min;


struct ConstantPlayerStub final : public game::Player
{

    explicit ConstantPlayerStub(
        std::string_view const player_name = "Unnamed Player",
        PlayerColor const pc = PlayerColor::dark,
        grid::Position played_position = grid::Position{grid::Row{0}, grid::Column{0}})
        : played_position{played_position}
    {
        name = player_name;
        color = pc;
    }


    [[nodiscard]] grid::Position pick_move(Board const& board) const override
    {
        return played_position;
    }

    grid::Position played_position;
};

struct MinimalPlayer final : public game::Player
{
    using Player::Player;

    [[nodiscard]] grid::Position pick_move(Board const& board) const override
    {
        auto moves = board.find_valid_moves(color);
        return min(moves);
    }
};


struct SpyForNotifierOutput final : public game::Notifier
{
    void display_message(std::string_view const message) override
    {
        stream << message << "\n";
    }

    [[nodiscard]] std::string output() const { return stream.str(); }

    std::stringstream stream{};
};

struct SpyForNotifierMoves final : public game::Notifier
{
    void display_message(std::string_view message) override
    {
        messages.emplace_back(message);
    }

    void display_board(Board const& board) override { boards.push_back(board); }

    void note_new_game(game::Players const& players, Board board) override {}

    void note_move(
        game::Player const& player, grid::Position pos, Board const& board) override
    {
        moves.emplace_back(
            player.get_name().data(),
            player.get_color(),
            pos.get_row(),
            pos.get_column());
    }

    void note_result(game::GameResult const& result) override
    {
        if (auto* win_result = dynamic_cast<game::WinByScore const*>(&result)) {
            result_summary.type = "win";
            result_summary.winner = win_result->get_winner().get_color();
            result_summary.loser = win_result->get_loser().get_color();
        }
        else if (auto* tie_result = dynamic_cast<game::TiedResult const*>(&result)) {
            result_summary.type = "tie";
            result_summary.winner = tie_result->get_dark_player().get_color();
            result_summary.loser = tie_result->get_light_player().get_color();
        }
        else if (
            auto* goof_result
            = dynamic_cast<game::WinByOpponentMistake const*>(&result)) {
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
    std::vector<Board> boards{};
    std::vector<Move> moves{};
    Result result_summary{};
};

inline bool
operator==(SpyForNotifierMoves::Move const& lhs, SpyForNotifierMoves::Move const& rhs)
{
    return lhs.player_name == rhs.player_name && lhs.color == rhs.color
           && lhs.row == rhs.row && lhs.column == rhs.column;
}

inline std::ostream& operator<<(std::ostream& os, SpyForNotifierMoves const& spy)
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
