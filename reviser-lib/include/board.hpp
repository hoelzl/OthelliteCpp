// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once
#ifndef REVISER_LIB_BOARD_HPP
#define REVISER_LIB_BOARD_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <iterator>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "common.hpp"
#include "position.hpp"

namespace reviser {

namespace rng = ::std::ranges;

enum class InitialBoardState
{
    empty,
    center_square,
};

[[nodiscard]] const std::vector<Position>& all_board_positions();

class BasicBoard
{
public:
    BasicBoard() = default;
    BasicBoard(const BasicBoard& other) = delete;
    BasicBoard(BasicBoard&& other) noexcept = delete;
    BasicBoard& operator=(const BasicBoard& other) = delete;
    BasicBoard& operator=(BasicBoard&& other) noexcept = delete;
    virtual ~BasicBoard() = default;

    using Moves = std::set<Position>;
    using OrderedMoves = std::vector<Position>;
    using Positions = std::set<Position>;
    using OrderedPositions = std::vector<Position>;

    virtual Field& operator[](Position pos) = 0;
    virtual const Field& operator[](Position pos) const = 0;

    [[nodiscard]] virtual std::string to_string() const = 0;

    virtual void
    initialize(InitialBoardState initial_state = InitialBoardState::center_square)
        = 0;

    [[nodiscard]] virtual bool is_empty(Position pos) const = 0;
    [[nodiscard]] virtual bool is_occupied(Position pos) const = 0;

    [[nodiscard]] virtual bool is_valid_move(PlayerColor pc, Position pos) const = 0;

    [[nodiscard]] virtual Moves find_valid_moves(PlayerColor pc) const = 0;

    virtual void play_move(PlayerColor pc, Position pos) = 0;

    [[nodiscard]] virtual Score compute_score() const = 0;
};

template <typename BoardT>
concept BasicBoardType = requires(
    BoardT b,
    const BoardT cb,
    std::string s,
    Position pos,
    InitialBoardState initial_state,
    PlayerColor pc)
{
    // clang-format off
    typename BoardT::Moves;
    { b.operator[](pos) } -> std::convertible_to<Field&>;
    { cb.operator[](pos) } -> std::convertible_to<const Field&>;
    { cb.to_string() } -> std::convertible_to<std::string>;
    b.initialize();
    b.initialize(initial_state);
    { b.is_empty(pos) } -> std::convertible_to<bool>;
    { b.is_occupied(pos) } -> std::convertible_to<bool>;
    { b.is_valid_move(pc, pos) } -> std::convertible_to<bool>;
    { b.find_valid_moves(pc) } -> std::convertible_to<typename BoardT::Moves>;
    b.play_move(pc, pos);
    { b.compute_score() } -> std::convertible_to<Score>;
    // clang-format on
};

static_assert(BasicBoardType<BasicBoard>);

template <typename BoardT>
// clang-format off
concept BoardType = BasicBoardType<BoardT> && requires(BoardT b, std::string s)
{
    BoardT{};
    std::forward_iterator<typename BoardT::iterator>;
    typename BoardT::Moves;
    //{ BoardT::from_string(s) } -> std::convertible_to<BoardT>;
    { ::std::begin(b) } -> std::convertible_to<typename BoardT::iterator>;
    { ::std::end(b) } -> std::convertible_to<typename BoardT::iterator>;
};
// clang-format on


template <BoardType Board>
class BoardReader
{
public:
    static Board board_from_string(std::string_view board_str)
    {
        const auto cleaned_string = clean_board_str(board_str);
        assert(cleaned_string.size() == 64);
        auto result = Board{};
        for (auto i = 0u; i < 64; ++i) {
            result[i] = convert_char(cleaned_string[i]);
        }
        return result;
    }

    static std::string clean_board_str(std::string_view board_str)
    {
        static const auto valid_chars = std::string{"O* "};
        auto result = std::string{};
        rng::copy_if(board_str, std::back_inserter(result), [](auto c) {
            return c == 'O' || c == '*' || c == ' ';
        });
        return result;
    }

    static Field convert_char(char c)
    {
        switch (c) {
        case 'O': return Field::light;
        case '*': return Field::dark;
        case ' ': return Field::empty;
        default:
            throw std::invalid_argument("Field inputs can only be ' ', '*' or 'O'.");
        }
    }
};

template <BoardType Board>
class BoardWriter
{
public:
    static std::string board_to_string(const Board& board)
    {
        auto result = std::ostringstream{};
        auto prefix = std::string{"|"};
        for (auto pos : all_board_positions()) {
            result << prefix << field_to_char(board[pos]);
            if (pos.get_column() == 7) {
                prefix = std::string{"|\n|"};
            }
            else {
                prefix = std::string{"|"};
            }
        }
        result << std::string{"|"};
        return result.str();
    }
};

} // namespace reviser
#endif // REVISER_LIB_BOARD_HPP
