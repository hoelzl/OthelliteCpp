// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "array_board.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <map>

#include "common.hpp"
#include "direction.hpp"
#include "position.hpp"


namespace reviser {

using grid::Column;
using grid::Direction;
using grid::Position;
using grid::Row;
using ::std::ranges::copy_if;

auto ArrayBoard::from_string(const std::string_view board_string) -> ArrayBoard
{
    return BoardReader<ArrayBoard>::board_from_string(board_string);
}

auto ArrayBoard::operator[](const std::size_t index) -> Field&
{
    assert(index < 64);
    return fields.at(index);
}

auto ArrayBoard::operator[](const grid::Position pos) -> Field&
{
    return fields.at(pos.to_linear_index());
}

const Field& ArrayBoard::operator[](const grid::Position pos) const
{
    return fields.at(pos.to_linear_index());
}

std::string ArrayBoard::to_string() const
{
    return BoardWriter<ArrayBoard>::board_to_string(*this);
}

bool ArrayBoard::is_empty(Position pos) const { return field_is_empty((*this)[pos]); }

[[maybe_unused]] bool ArrayBoard::is_occupied(const Position pos) const
{
    return field_is_occupied((*this)[pos]);
}

bool ArrayBoard::is_valid_move(const PlayerColor pc, const Position pos) const
{
    return is_empty(pos) && does_move_flip_any_field(pc, pos);
}

bool ArrayBoard::does_move_flip_any_field(PlayerColor pc, Position starting_pos) const
{
    return std::ranges::any_of(grid::directions, [&](auto d) {
        return !positions_to_flip_in_direction(pc, starting_pos, d).empty();
    });
}

ArrayBoard::Positions ArrayBoard::positions_to_flip_in_direction(
    const PlayerColor pc, const Position starting_pos, const Direction d) const
{
    const auto next_pos = starting_pos.next_in_direction(d);
    const auto occupied_positions = occupied_positions_in_direction(d, next_pos);
    return filter_positions_that_can_be_flipped(pc, occupied_positions);
}

ArrayBoard::OrderedPositions ArrayBoard::occupied_positions_in_direction(
    const Direction d, Position starting_pos) const
{
    auto occupied_positions = std::vector<Position>{};
    while (starting_pos.is_valid()) {
        if (!field_is_occupied((*this)[starting_pos])) {
            break;
        }
        occupied_positions.push_back(starting_pos);
        starting_pos = starting_pos.next_in_direction(d);
    }
    return occupied_positions;
}

ArrayBoard::Positions ArrayBoard::filter_positions_that_can_be_flipped(
    const PlayerColor pc, const OrderedPositions& non_empty_positions) const
{
    auto result = std::set<Position>{};
    const auto highest_index
        = find_highest_index_for_player_owned_fields(pc, non_empty_positions);

    for (auto i = 0u; i < highest_index; ++i) {
        auto& pos = non_empty_positions[i];
        if (field_is_owned_by_opponent_of((*this)[pos], pc)) {
            result.insert(pos);
        }
    }

    return result;
}

std::size_t ArrayBoard::find_highest_index_for_player_owned_fields(
    const PlayerColor pc, const OrderedPositions& non_empty_positions) const
{
    const auto num_non_empty_position = static_cast<int>(non_empty_positions.size());
    for (int i = num_non_empty_position - 1; i >= 0; --i) {
        const auto& position = Position{non_empty_positions[i]};
        if (const auto& field = (*this)[position];
            field_is_owned_by_player(field, pc)) {
            return i;
        }
    }
    return 0;
}

void ArrayBoard::initialize(const InitialBoardState initial_state)
{
    for (const auto pos : all_board_positions()) {
        (*this)[pos] = Field::empty;
    }
    if (initial_state == InitialBoardState::center_square) {
        (*this)[Position{Row{3}, Column{3}}] = Field::dark;
        (*this)[Position{Row{3}, Column{4}}] = Field::light;
        (*this)[Position{Row{4}, Column{3}}] = Field::light;
        (*this)[Position{Row{4}, Column{4}}] = Field::dark;
    }
}

ArrayBoard::Moves ArrayBoard::find_valid_moves(const PlayerColor pc) const
{
    auto result = std::set<Position>{};
    for (auto pos : all_board_positions()) {
        if (is_valid_move(pc, pos)) {
            result.insert(pos);
        }
    }
    return result;
}

void ArrayBoard::play_move(const PlayerColor pc, const Position pos)
{
    if (is_valid_move(pc, pos)) {
        (*this)[pos] = field_for_player_color(pc);
        const auto flipped_positions = find_positions_flipped_by_move(pc, pos);
        flip_positions(pc, flipped_positions);
    }
}

Score ArrayBoard::compute_score() const
{
    std::map<Field, int_fast8_t> counts;
    for (const auto pos : all_board_positions()) {
        ++counts[(*this)[pos]];
    }
    return Score{counts};
}

ArrayBoard::Positions ArrayBoard::find_positions_flipped_by_move(
    const PlayerColor pc, const Position pos) const
{
    auto result = std::set<Position>{};
    for (const auto d : grid::directions) {
        result.merge(positions_to_flip_in_direction(pc, pos, d));
    }
    return result;
}

void ArrayBoard::flip_positions(
    const PlayerColor pc, const ArrayBoard::Positions& positions_to_flip)
{
    const auto field = field_for_player_color(pc);
    for (const auto pos : positions_to_flip) {
        (*this)[pos] = field;
    }
}


bool operator==(const ArrayBoard& lhs, const ArrayBoard& rhs)
{
    return std::ranges::all_of(
        all_board_positions(), [&](auto p) { return lhs[p] == rhs[p]; });
}

} // namespace reviser
