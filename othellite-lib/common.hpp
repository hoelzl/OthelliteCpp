// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once
#ifndef OTHELLITE_COMMON_HPP
#define OTHELLITE_COMMON_HPP

#include <cstdint>
#include <stdexcept>

namespace othellite {

constexpr int_least8_t board_size{8};

enum class Field : int_least8_t
{
    empty,
    light,
    dark,
};

enum class PlayerColor : int_least8_t
{
    light,
    dark,
};

bool field_is_empty(Field field);
bool field_is_occupied(Field field);

Field field_for_player_color(PlayerColor pc);
bool field_is_owned_by_player(Field field, PlayerColor pc);
bool field_is_owned_by_opponent_of(Field field, PlayerColor pc);

char field_to_char(Field field);

PlayerColor other_player_color(PlayerColor pc);

} // namespace othellite

#endif // OTHELLITE_COMMON_HPP
