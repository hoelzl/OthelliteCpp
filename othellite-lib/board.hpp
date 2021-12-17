// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once
#ifndef OTHELLITE_LIB_BOARD_HPP
#define OTHELLITE_LIB_BOARD_HPP

#include <array>
#include <concepts>
#include <set>
#include <string>
#include <vector>

#include "common.hpp"
#include "position.hpp"

namespace othellite {

enum class InitialBoardState
{
    empty,
    center_square,
};

class Board
{
    std::array<Field, 64> fields{};

public:
    using iterator = decltype(fields)::iterator;
    using const_iterator [[maybe_unused]] = decltype(fields)::const_iterator;

    Board() = default;

    iterator begin() { return std::begin(fields); }
    iterator end() { return std::end(fields); }

    static Board from_string(std::string_view board_string);

    Field& operator[](grid::Position pos);
    Field const& operator[](grid::Position pos) const;

    [[nodiscard]] std::string to_string() const;

    void initialize(InitialBoardState initial_state = InitialBoardState::center_square);

    [[nodiscard]] bool is_empty(grid::Position pos) const;
    [[maybe_unused]] [[nodiscard]] bool is_occupied(grid::Position pos) const;

    [[nodiscard]] bool is_valid_move(PlayerColor pc, grid::Position pos) const;

    [[nodiscard]] std::set<grid::Position> find_valid_moves(PlayerColor pc) const;

    void play_move(PlayerColor pc, grid::Position pos);

    [[nodiscard]] Score compute_score() const;

private:
    friend class BoardReader;
    friend class BoardWriter;

    Field& operator[](std::size_t index);

    [[nodiscard]] bool
    does_move_flip_any_field(PlayerColor pc, grid::Position starting_pos) const;

    [[nodiscard]] std::set<grid::Position> positions_to_flip_in_direction(
        PlayerColor pc, grid::Position starting_pos, grid::Direction d) const;

    [[nodiscard]] std::vector<grid::Position> occupied_positions_in_direction(
        grid::Direction d, grid::Position starting_pos) const;

    [[nodiscard]] std::set<grid::Position> filter_positions_that_can_be_flipped(
        PlayerColor pc, std::vector<grid::Position> const& non_empty_positions) const;

    [[nodiscard]] std::size_t find_highest_index_for_player_owned_fields(
        PlayerColor pc, std::vector<grid::Position> const& non_empty_positions) const;

    [[nodiscard]] std::set<grid::Position>
    find_positions_flipped_by_move(PlayerColor pc, grid::Position pos) const;

    void
    flip_positions(PlayerColor pc, std::set<grid::Position> const& positions_to_flip);
};

bool operator==(Board const& lhs, Board const& rhs);

[[nodiscard]] std::vector<grid::Position> const& all_board_positions();

template <typename BoardT>
concept BoardType = requires(
    BoardT b,
    BoardT const cb,
    std::string s,
    grid::Position pos,
    InitialBoardState initial_state,
    PlayerColor pc)
{
    // clang-format off
	std::forward_iterator<typename BoardT::iterator>;
    { BoardT::from_string(s) } -> std::convertible_to<BoardT>;
	{ ::std::begin(b) } -> std::convertible_to<typename BoardT::iterator>;
	{ ::std::end(b) } -> std::convertible_to<typename BoardT::iterator>;
	{ b.operator[](pos) } -> std::convertible_to<Field&>;
	{ cb.operator[](pos) } -> std::convertible_to<Field const&>;
	{ cb.to_string() } -> std::convertible_to<std::string>;
	b.initialize();
	b.initialize(initial_state);
	{ b.is_empty(pos) } -> std::convertible_to<bool>;
	{ b.is_occupied(pos) } -> std::convertible_to<bool>;
	{ b.is_valid_move(pc, pos) } -> std::convertible_to<bool>;
	{ b.find_valid_moves(pc) } -> std::convertible_to<std::set<grid::Position>>;
	b.play_move(pc, pos);
	{ b.compute_score() } -> std::convertible_to<Score>;
    // clang-format on
};

static_assert(BoardType<Board>);

class BoardReader
{
public:
    static Board board_from_string(std::string_view board_str);
    static std::string clean_board_str(std::string_view board_str);
    static Field convert_char(char c);
};

class BoardWriter
{
public:
    static std::string board_to_string(Board const& board);
};

} // namespace othellite
#endif // OTHELLITE_LIB_BOARD_HPP
