// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once
#ifndef REVISER_LIB_POSITION_HPP
#define REVISER_LIB_POSITION_HPP

#include <cstdint>
#include <format>
#include <stdexcept>

#include "common.hpp"
#include "direction.hpp"

namespace reviser::grid {

using coordinate_t = int_least8_t;

struct Row
{
    constexpr explicit Row(const int row)
        : value{static_cast<coordinate_t>(row)}
    {}

    coordinate_t value;

    // ReSharper disable once CppNonExplicitConversionOperator
    constexpr operator coordinate_t() const // NOLINT(google-explicit-constructor)
    {
        return value;
    }
};

inline constexpr bool operator==(const Row lhs, const Row rhs)
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

inline constexpr bool operator==(const Column lhs, const Column rhs)
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

    [[nodiscard]] constexpr Position next_in_direction(const Direction d) const
    {
        throw_if_invalid();
        return {Row{row + d.get_dy()}, Column{column + d.get_dx()}};
    }

    [[nodiscard]] std::size_t to_linear_index() const
    {
        return row * board_size + column;
    }

	[[nodiscard]] std::string to_string() const
    {
        return std::format("({}, {})", get_row().value, get_column().value);
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

inline constexpr bool operator==(const Position lhs, const Position rhs)
{
    return lhs.get_row() == rhs.get_row() && lhs.get_column() == rhs.get_column();
}

inline constexpr bool operator<(const Position lhs, const Position rhs)
{
    return lhs.get_row() < rhs.get_row()
           || (lhs.get_row() == rhs.get_row() && lhs.get_column() < rhs.get_column());
}

inline constexpr bool operator<=(const Position lhs, const Position rhs)
{
    return lhs == rhs || lhs < rhs;
}

inline constexpr bool operator>(const Position lhs, const Position rhs)
{
    return rhs < lhs;
}

inline constexpr bool operator>=(const Position lhs, const Position rhs)
{
    return rhs <= lhs;
}

std::ostream& operator<<(std::ostream& os, Position pos);

} // namespace reviser::grid


#endif // REVISER_LIB_POSITION_HPP
