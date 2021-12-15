// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once
#ifndef OTHELLITE_BOARD_HPP
#define OTHELLITE_BOARD_HPP

#include <array>
#include <set>
#include <string>
#include <vector>

#include "common.hpp"
#include "position.hpp"

namespace othellite {

class Board
{
    std::array<Field, 64> fields{};

public:
    enum class InitialState
    {
        empty,
        center_square,
    };

    using iterator = decltype(fields)::iterator;
    using const_iterator [[maybe_unused]] = decltype(fields)::const_iterator;

    iterator begin() { return std::begin(fields); }
    iterator end() { return std::end(fields); }

    static Board from_string(std::string_view board_string);

    Field& operator[](grid::Position pos);
    Field const& operator[](grid::Position pos) const;

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] static std::vector<grid::Position> const& get_positions();

    void initialize(InitialState initial_state = InitialState::center_square);

    [[nodiscard]] bool is_empty(grid::Position pos) const;
    [[maybe_unused]] [[nodiscard]] bool is_occupied(grid::Position pos) const;

    [[nodiscard]] bool is_valid_move(PlayerColor pc, grid::Position pos) const;

    [[nodiscard]] std::set<grid::Position> find_valid_moves(PlayerColor pc) const;

    void play_move(PlayerColor pc, grid::Position pos);

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
#endif // OTHELLITE_BOARD_HPP
