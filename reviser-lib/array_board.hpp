// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once
#ifndef REVISER_LIB_ARRAY_BOARD_HPP
#define REVISER_LIB_ARRAY_BOARD_HPP

#include <array>
#include <set>
#include <string>
#include <vector>

#include "board.hpp"
#include "common.hpp"
#include "position.hpp"

namespace reviser {

class ArrayBoard
{
    std::array<Field, 64> fields{};

public:
    using iterator = decltype(fields)::iterator;
    using const_iterator = decltype(fields)::const_iterator;
    using Moves = std::set<grid::Position>;
    using OrderedMoves = std::vector<grid::Position>;
    using Positions = std::set<grid::Position>;
    using OrderedPositions = std::vector<grid::Position>;


    ArrayBoard() = default;

    iterator begin() { return std::begin(fields); }
    iterator end() { return std::end(fields); }

    static ArrayBoard from_string(std::string_view board_string);

    Field& operator[](grid::Position pos);
    const Field& operator[](grid::Position pos) const;

    [[nodiscard]] std::string to_string() const;

    void initialize(InitialBoardState initial_state = InitialBoardState::center_square);

    [[nodiscard]] bool is_empty(grid::Position pos) const;
    [[maybe_unused]] [[nodiscard]] bool is_occupied(grid::Position pos) const;

    [[nodiscard]] bool is_valid_move(PlayerColor pc, grid::Position pos) const;

    [[nodiscard]] Moves find_valid_moves(PlayerColor pc) const;

    void play_move(PlayerColor pc, grid::Position pos);

    [[nodiscard]] Score compute_score() const;

private:
    template <typename T> friend class BoardReader;
    template <typename T> friend class BoardWriter;

    Field& operator[](std::size_t index);

    [[nodiscard]] bool
    does_move_flip_any_field(PlayerColor pc, grid::Position starting_pos) const;

    [[nodiscard]] Positions positions_to_flip_in_direction(
        PlayerColor pc, grid::Position starting_pos, grid::Direction d) const;

    [[nodiscard]] OrderedPositions occupied_positions_in_direction(
        grid::Direction d, grid::Position starting_pos) const;

    [[nodiscard]] Positions filter_positions_that_can_be_flipped(
        PlayerColor pc, const OrderedPositions& non_empty_positions) const;

    [[nodiscard]] std::size_t find_highest_index_for_player_owned_fields(
        PlayerColor pc, const OrderedPositions& non_empty_positions) const;

    [[nodiscard]] Positions
    find_positions_flipped_by_move(PlayerColor pc, grid::Position pos) const;

    void flip_positions(PlayerColor pc, const Positions& positions_to_flip);
};

bool operator==(const ArrayBoard& lhs, const ArrayBoard& rhs);

static_assert(BoardType<ArrayBoard>);

} // namespace reviser
#endif // REVISER_LIB_ARRAY_BOARD_HPP
