// Copyright (c) 2021-2023 Dr. Matthias Hölzl.

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

    class ArrayBoard final : public BasicBoard {
    private:
        std::array<Field, 64> fields{};

    public:
        using iterator = decltype(fields)::iterator;
        using const_iterator [[maybe_unused]] = decltype(fields)::const_iterator;
        using Moves = std::set<Position>;
        using OrderedMoves [[maybe_unused]] = std::vector<Position>;
        using Positions = std::set<Position>;
        using OrderedPositions = std::vector<Position>;


        ArrayBoard() = default;

        ArrayBoard(const ArrayBoard &other)
                : fields{other.fields} {}

        ArrayBoard(ArrayBoard &&other) noexcept
                : fields{other.fields} {}

        ArrayBoard &operator=(const ArrayBoard &other) {
            if (this == &other) {
                return *this;
            }
            fields = other.fields;
            return *this;
        }

        ArrayBoard &operator=(ArrayBoard &&other) noexcept {
            if (this == &other) {
                return *this;
            }
            fields = other.fields;
            return *this;
        }

        ~ArrayBoard() override = default;

        iterator begin() { return std::begin(fields); }

        iterator end() { return std::end(fields); }

        static ArrayBoard from_string(std::string_view board_string);

        Field &operator[](Position pos) override;

        const Field &operator[](Position pos) const override;

        [[nodiscard]] std::string to_string() const override;

        void initialize(
                InitialBoardState initial_state = InitialBoardState::center_square) override;

        [[nodiscard]] bool is_empty(Position pos) const override;

        [[maybe_unused]] [[nodiscard]] bool is_occupied(Position pos) const override;

        [[nodiscard]] bool is_valid_move(PlayerColor pc, Position pos) const override;

        [[nodiscard]] Moves find_valid_moves(PlayerColor pc) const override;

        void play_move(PlayerColor pc, Position pos) override;

        [[nodiscard]] Score compute_score() const override;

    private:
        template<BoardType Board>
        friend
        class BoardReader;

        template<BoardType Board>
        friend
        class BoardWriter;

        Field &operator[](std::size_t index);

        [[nodiscard]] bool
        does_move_flip_any_field(PlayerColor pc, Position starting_pos) const;

        [[nodiscard]] Positions positions_to_flip_in_direction(
                PlayerColor pc, Position starting_pos, Direction d) const;

        [[nodiscard]] OrderedPositions
        occupied_positions_in_direction(Direction d, Position starting_pos) const;

        [[nodiscard]] Positions filter_positions_that_can_be_flipped(
                PlayerColor pc, const OrderedPositions &non_empty_positions) const;

        [[nodiscard]] std::size_t find_highest_index_for_player_owned_fields(
                PlayerColor pc, const OrderedPositions &non_empty_positions) const;

        [[nodiscard]] Positions
        find_positions_flipped_by_move(PlayerColor pc, Position pos) const;

        void flip_positions(PlayerColor pc, const Positions &positions_to_flip);
    };

    bool operator==(const ArrayBoard &lhs, const ArrayBoard &rhs);

    static_assert(BasicBoardType<ArrayBoard>);
    static_assert(BoardType<ArrayBoard>);

} // namespace reviser
#endif // REVISER_LIB_ARRAY_BOARD_HPP
