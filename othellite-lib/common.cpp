// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "common.hpp"

bool othellite::field_is_empty(othellite::Field field) { return field == Field::empty; }
bool othellite::field_is_occupied(othellite::Field field)
{
    return field != Field::empty;
}

auto othellite::field_for_player_color(PlayerColor pc) -> Field
{
    switch (pc) {
        case PlayerColor::light: return Field::light;
        case PlayerColor::dark: return Field::dark;
        default: throw std::invalid_argument("Unknown player color.");
    }
}

bool othellite::field_is_owned_by_player(Field field, PlayerColor pc)
{
    return field == field_for_player_color(pc);
}
bool othellite::field_is_owned_by_opponent_of(Field field, PlayerColor pc)
{
    return field_is_occupied(field) && !field_is_owned_by_player(field, pc);
}

othellite::PlayerColor othellite::other_player_color(othellite::PlayerColor pc)
{
    if (pc == PlayerColor::light) {
        return PlayerColor::dark;
    } else {
        return PlayerColor::light;
    }
}
char othellite::field_to_char(othellite::Field field)
{
    switch (field) {
        case Field::light: return 'O';
        case Field::dark: return '*';
        case Field::empty: return ' ';
        default: throw std::invalid_argument("Unknown field type.");
    }
}
