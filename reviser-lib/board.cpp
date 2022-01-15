// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "board.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <stdexcept>

#include "common.hpp"
#include "direction.hpp"
#include "position.hpp"


namespace reviser {

using grid::Column;
using grid::Direction;
using grid::Position;
using grid::Row;
using ::std::ranges::copy_if;

auto Board::from_string(const std::string_view board_string) -> Board
{
    return BoardReader::board_from_string(board_string);
}

auto Board::operator[](const std::size_t index) -> Field&
{
    assert(index < 64);
    return fields[index];
}

auto Board::operator[](const grid::Position pos) -> Field&
{
    return fields[pos.to_linear_index()];
}

const Field& Board::operator[](const grid::Position pos) const
{
    return fields[pos.to_linear_index()];
}

std::string Board::to_string() const { return BoardWriter::board_to_string(*this); }

bool Board::is_empty(Position pos) const { return field_is_empty((*this)[pos]); }

[[maybe_unused]] bool Board::is_occupied(const Position pos) const
{
    return field_is_occupied((*this)[pos]);
}

bool Board::is_valid_move(const PlayerColor pc, const Position pos) const
{
    return is_empty(pos) && does_move_flip_any_field(pc, pos);
}

bool Board::does_move_flip_any_field(PlayerColor pc, Position starting_pos) const
{
    return std::ranges::any_of(grid::directions, [&](auto d) {
        return !positions_to_flip_in_direction(pc, starting_pos, d).empty();
    });
}

std::set<Position> Board::positions_to_flip_in_direction(
    const PlayerColor pc, const Position starting_pos, const Direction d) const
{
    const auto next_pos = starting_pos.next_in_direction(d);
    const auto occupied_positions = occupied_positions_in_direction(d, next_pos);
    return filter_positions_that_can_be_flipped(pc, occupied_positions);
}

std::vector<Position>
Board::occupied_positions_in_direction(const Direction d, Position starting_pos) const
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

std::set<Position> Board::filter_positions_that_can_be_flipped(
    const PlayerColor pc, const std::vector<Position>& non_empty_positions) const
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

std::size_t Board::find_highest_index_for_player_owned_fields(
    const PlayerColor pc, const std::vector<Position>& non_empty_positions) const
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

void Board::initialize(const InitialBoardState initial_state)
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

std::set<Position> Board::find_valid_moves(const PlayerColor pc) const
{
    auto result = std::set<Position>{};
    for (auto pos : all_board_positions()) {
        if (is_valid_move(pc, pos)) {
            result.insert(pos);
        }
    }
    return result;
}

void Board::play_move(const PlayerColor pc, const Position pos)
{
    if (is_valid_move(pc, pos)) {
        (*this)[pos] = field_for_player_color(pc);
        const auto flipped_positions = find_positions_flipped_by_move(pc, pos);
        flip_positions(pc, flipped_positions);
    }
}

Score Board::compute_score() const
{
    int_fast8_t dark_count{0};
    int_fast8_t light_count{0};
    int_fast8_t empty_count{0};
    for (const auto pos : all_board_positions()) {
        switch ((*this)[pos]) {
        case Field::dark: ++dark_count; break;
        case Field::light: ++light_count; break;
        case Field::empty: ++empty_count; break;
        }
    }
    return Score{dark_count, light_count, empty_count};
}

std::set<Position>
Board::find_positions_flipped_by_move(const PlayerColor pc, const Position pos) const
{
    auto result = std::set<Position>{};
    for (const auto d : grid::directions) {
        result.merge(positions_to_flip_in_direction(pc, pos, d));
    }
    return result;
}

void Board::flip_positions(
    const PlayerColor pc, const std::set<Position>& positions_to_flip)
{
    const auto field = field_for_player_color(pc);
    for (const auto pos : positions_to_flip) {
        (*this)[pos] = field;
    }
}

auto BoardReader::board_from_string(const std::string_view board_str) -> Board
{
    const auto cleaned_string = clean_board_str(board_str);
    assert(cleaned_string.size() == 64);
    auto result = Board{};
    for (auto i = 0u; i < 64; ++i) {
        result[i] = convert_char(cleaned_string[i]);
    }
    return result;
}

std::string BoardReader::clean_board_str(std::string_view board_str)
{
    static const auto valid_chars = std::string{"O* "};
    auto result = std::string{};
    copy_if(board_str, std::back_inserter(result), [](auto c) {
        return c == 'O' || c == '*' || c == ' ';
    });
    return result;
}

auto BoardReader::convert_char(const char c) -> Field
{
    switch (c) {
    case 'O': return Field::light;
    case '*': return Field::dark;
    case ' ': return Field::empty;
    default: throw std::invalid_argument("Field inputs can only be ' ', '*' or 'O'.");
    }
}

std::string BoardWriter::board_to_string(const Board& board)
{
    auto result = std::string{};
    auto prefix = std::string{"|"};
    for (auto pos : all_board_positions()) {
        const auto next_char = field_to_char(board[pos]);
        result += prefix + std::string{next_char};
        if (pos.get_column() == 7) {
            prefix = std::string{"|\n|"};
        }
        else {
            prefix = std::string{"|"};
        }
    }
    result += std::string{"|"};
    return result;
}

bool operator==(const Board& lhs, const Board& rhs)
{
    return std::ranges::all_of(
        all_board_positions(), [&](auto p) { return lhs[p] == rhs[p]; });
}

const std::vector<Position>& all_board_positions()
{
    static auto result = std::vector<Position>{};
    if (result.empty()) {
        for (auto row = 0; row < 8; ++row) {
            for (auto col = 0; col < 8; ++col) {
                result.push_back(Position{Row{row}, Column{col}});
            }
        }
    }
    return result;
}
} // namespace reviser
