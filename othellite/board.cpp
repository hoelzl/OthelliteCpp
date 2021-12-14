// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "board.hpp"
#include "common.hpp"
#include "position.hpp"
#include <algorithm>
#include <cassert>
#include <iterator>
#include <stdexcept>

using othellite::field_to_char;
using othellite::grid::Column;
using othellite::grid::Position;
using othellite::grid::Row;
using std::ranges::copy_if;
using std::ranges::transform;

auto othellite::Board::from_string(std::string_view board_string) -> Board
{
    return BoardReader::board_from_string(board_string);
}

auto othellite::Board::operator[](std::size_t index) -> Field&
{
    assert(index < 64);
    return fields[index];
}

auto othellite::Board::operator[](othellite::grid::Position pos) -> Field&
{
    return fields[pos.to_linear_index()];
}

const othellite::Field&
othellite::Board::operator[](othellite::grid::Position pos) const
{
    return fields[pos.to_linear_index()];
}

std::string othellite::Board::to_string() const
{
    return BoardWriter::board_to_string(*this);
}

auto othellite::BoardReader::board_from_string(std::string_view board_str) -> Board
{
    auto cleaned_string = clean_board_str(board_str);
    assert(cleaned_string.size() == 64);
    auto result = Board{};
    for (auto i = 0u; i < 64; ++i) {
        result[i] = convert_char(cleaned_string[i]);
    }
    return result;
}

std::string othellite::BoardReader::clean_board_str(std::string_view board_str)
{
    static const std::string valid_chars{"O* "};
    std::string result{};
    copy_if(board_str, std::back_inserter(result),
            [](auto c) { return c == 'O' || c == '*' || c == ' '; });
    return result;
}
auto othellite::BoardReader::convert_char(char c) -> Field
{
    switch (c) {
        case 'O': return Field::light;
        case '*': return Field::dark;
        case ' ': return Field::empty;
        default:
            throw std::invalid_argument("Field inputs can only be ' ', '*' or 'O'.");
    }
}
std::string othellite::BoardWriter::board_to_string(const Board& board)
{
    std::string result{};
    auto prefix = std::string{"|"};
    for (auto row = 0; row < 8; ++row) {
        for (auto col = 0; col < 8; ++col) {
            auto next_char = field_to_char(board[Position{Row{row}, Column{col}}]);
            result += prefix + std::string{next_char};
            prefix = std::string{"|"};
        }
        prefix = std::string{"|\n|"};
    }
    result += std::string{"|"};
    return result;
}
