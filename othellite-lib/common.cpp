// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "common.hpp"

#include <stdexcept>

bool othellite::field_is_empty(othellite::Field const field)
{
    return field == Field::empty;
}

bool othellite::field_is_occupied(othellite::Field const field)
{
    return field != Field::empty;
}

auto othellite::field_for_player_color(PlayerColor const pc) -> Field
{
    switch (pc) {
    case PlayerColor::light: return Field::light;
    case PlayerColor::dark: return Field::dark;
    }
    throw std::invalid_argument("Unexpected value for player color.");
}

bool othellite::field_is_owned_by_player(Field const field, PlayerColor const pc)
{
    return field == field_for_player_color(pc);
}

bool othellite::field_is_owned_by_opponent_of(Field const field, PlayerColor const pc)
{
    return field_is_occupied(field) && !field_is_owned_by_player(field, pc);
}

auto othellite::other_player_color(othellite::PlayerColor const pc) -> PlayerColor
{
    if (pc == PlayerColor::light) {
        return PlayerColor::dark;
    }
    else {
        return PlayerColor::light;
    }
}
char othellite::field_to_char(othellite::Field const field)
{
    switch (field) {
    case Field::light: return 'O';
    case Field::dark: return '*';
    case Field::empty: return ' ';
    }
    throw std::invalid_argument("Unexpected value for field.");
}
