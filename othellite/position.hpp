// Copyright (c) 2021 Dr. Matthias Hölzl.

#ifndef OTHELLITE_POSITION_HPP
#define OTHELLITE_POSITION_HPP

#include "common.hpp"
#include "direction.hpp"
#include <cstdint>
#include <stdexcept>

namespace othellite::grid {

using coordinate_t = int_least8_t;

struct Row {
    constexpr explicit Row(int row) : value{static_cast<coordinate_t>(row)} {}

    coordinate_t value;
    constexpr operator coordinate_t() const // NOLINT(google-explicit-constructor)
    {
        return value;
    }
};

inline constexpr bool operator==(Row lhs, Row rhs) { return lhs.value == rhs.value; }

struct Column {
    constexpr explicit Column(int column) : value{static_cast<coordinate_t>(column)} {}

    coordinate_t value;
    constexpr operator coordinate_t() const // NOLINT(google-explicit-constructor)
    {
        return value;
    }
};

inline constexpr bool operator==(Column lhs, Column rhs)
{
    return lhs.value == rhs.value;
}

class Position
{
public:
    constexpr Position(Row row, Column column) : row{row}, column{column} {}

    [[nodiscard]] constexpr Row get_row() const { return row; }
    [[nodiscard]] constexpr Column get_column() const { return column; }

    [[nodiscard]] constexpr bool is_valid() const
    {
        return 0 <= row && row < board_size && 0 <= column && column < board_size;
    }

    constexpr Position next_in_direction(Direction d)
    {
        throw_if_invalid();
        return {Row{row + d.get_dy()}, Column{column + d.get_dx()}};
    }

    [[nodiscard]] std::size_t to_linear_index() const
    {
        return row * board_size + column;
    }

private:
    Row row;
    Column column;

    constexpr void throw_if_invalid() const
    {
        if (!is_valid()) {
            throw std::invalid_argument("Invalid grid position!");
        }
    }
};

inline constexpr bool operator==(Position lhs, Position rhs)
{
    return lhs.get_row() == rhs.get_row() && lhs.get_column() == rhs.get_column();
}

std::ostream& operator<<(std::ostream& os, Position pos);

} // namespace othellite::grid


#endif // OTHELLITE_POSITION_HPP
