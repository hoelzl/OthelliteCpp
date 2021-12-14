// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once
#ifndef OTHELLITE_BOARD_HPP
#define OTHELLITE_BOARD_HPP

#include "common.hpp"
#include "position.hpp"
#include <array>
#include <concepts>
#include <memory>
#include <string>
#include <string_view>

namespace othellite {

class Board
{
    std::array<Field, 64> fields{};

public:
    using iterator = decltype(fields)::iterator;
    using const_iterator [[maybe_unused]] = decltype(fields)::const_iterator;

    iterator begin() { return std::begin(fields); }
    iterator end() { return std::end(fields); }

    static Board from_string(std::string_view board_string);

    Field& operator[](grid::Position pos);
    const Field& operator[](grid::Position pos) const;

    [[nodiscard]] std::string to_string() const;

private:
    friend class BoardReader;
    friend class BoardWriter;
    Field& operator[](std::size_t index);
};

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
    static std::string board_to_string(const Board& board);
};

} // namespace othellite
#endif // OTHELLITE_BOARD_HPP
