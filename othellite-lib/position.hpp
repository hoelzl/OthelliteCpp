// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once
#ifndef OTHELLITE_LIB_POSITION_HPP
#define OTHELLITE_LIB_POSITION_HPP

#include <cstdint>
#include <stdexcept>

#include "common.hpp"
#include "direction.hpp"

namespace othellite::grid {

using coordinate_t = int_least8_t;

struct Row
{
    constexpr explicit Row(int const row)
        : value{static_cast<coordinate_t>(row)}
    {}

    coordinate_t value;

    // ReSharper disable once CppNonExplicitConversionOperator
    constexpr operator coordinate_t() const // NOLINT(google-explicit-constructor)
    {
        return value;
    }
};

inline constexpr bool operator==(Row const lhs, Row const rhs)
{
    return lhs.value == rhs.value;
}

struct Column
{
    constexpr explicit Column(int column)
        : value{static_cast<coordinate_t>(column)}
    {}

    coordinate_t value;

    // ReSharper disable once CppNonExplicitConversionOperator
    constexpr operator coordinate_t() const // NOLINT(google-explicit-constructor)
    {
        return value;
    }
};

inline constexpr bool operator==(Column const lhs, Column const rhs)
{
    return lhs.value == rhs.value;
}

class Position
{
public:
    constexpr Position(Row row, Column column)
        : row{row}
        , column{column}
    {}

    [[nodiscard]] constexpr Row get_row() const { return row; }
    [[nodiscard]] constexpr Column get_column() const { return column; }

    [[nodiscard]] constexpr bool is_valid() const
    {
        return 0 <= row && row < board_size && 0 <= column && column < board_size;
    }

    [[nodiscard]] constexpr Position next_in_direction(Direction const d) const
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

inline constexpr bool operator==(Position const lhs, Position const rhs)
{
    return lhs.get_row() == rhs.get_row() && lhs.get_column() == rhs.get_column();
}

inline constexpr bool operator<(Position const lhs, Position const rhs)
{
    return lhs.get_row() < rhs.get_row() ||
        (lhs.get_row() == rhs.get_row() && lhs.get_column() < rhs.get_column());
}

inline constexpr bool operator<=(Position const lhs, Position const rhs)
{
    return lhs == rhs || lhs < rhs;
}

inline constexpr bool operator>(Position const lhs, Position const rhs)
{
    return rhs < lhs;
}

inline constexpr bool operator>=(Position const lhs, Position const rhs)
{
    return rhs <= lhs;
}

std::ostream& operator<<(std::ostream& os, Position pos);

} // namespace othellite::grid


#endif // OTHELLITE_LIB_POSITION_HPP
