// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "common.hpp"

#include <format>
#include <stdexcept>

#include "game.hpp"
#include "player.hpp"

namespace reviser {

std::ostream& operator<<(std::ostream& os, Field field)
{
    switch (field) {
    case Field::empty: os << "empty"; break;
    case Field::light: os << "light"; break;
    case Field::dark: os << "dark"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, PlayerColor pc)
{
    switch (pc) {
    case PlayerColor::light: os << "light"; break;
    case PlayerColor::dark: os << "dark"; break;
    }
    return os;
}

bool field_is_empty(const Field field) { return field == Field::empty; }

bool field_is_occupied(const Field field) { return field != Field::empty; }

auto field_for_player_color(const PlayerColor pc) -> Field
{
    switch (pc) {
    case PlayerColor::light: return Field::light;
    case PlayerColor::dark: return Field::dark;
    }
    throw std::invalid_argument("Unexpected value for player color.");
}

bool field_is_owned_by_player(const Field field, const PlayerColor pc)
{
    return field == field_for_player_color(pc);
}

bool field_is_owned_by_opponent_of(const Field field, const PlayerColor pc)
{
    return field_is_occupied(field) && !field_is_owned_by_player(field, pc);
}

char field_to_char(const Field field)
{
    switch (field) {
    case Field::light: return 'O';
    case Field::dark: return '*';
    case Field::empty: return ' ';
    }
    throw std::invalid_argument("Unexpected value for field.");
}

auto other_player_color(const PlayerColor pc) -> PlayerColor
{
    if (pc == PlayerColor::light) {
        return PlayerColor::dark;
    }
    else {
        return PlayerColor::light;
    }
}

std::string player_color_to_string(const PlayerColor pc)
{
    return pc == PlayerColor::dark ? "dark" : "light";
}

int_fast8_t Score::get_num_fields_for(const PlayerColor pc) const
{
    switch (pc) {
    case PlayerColor::dark: return num_dark_fields;
    case PlayerColor::light: return num_light_fields;
    }
    throw std::invalid_argument{"Bad player color."};
}

int_fast8_t Score::get_num_fields_for(const game::Player& player) const
{
    return get_num_fields_for(player.get_color());
}


std::string Score::to_string(const PlayerColor first_player) const
{
    return std::format(
        "{}:{}",
        get_num_fields_for(first_player),
        get_num_fields_for(other_player_color(first_player)));
}

std::string Score::to_string(const game::Player& player) const
{
    return to_string(player.get_color());
}

bool Score::is_tied() const { return get_num_dark_fields() == get_num_light_fields(); }

std::tuple<const game::Player&, const game::Player&>
Score::compute_winner(const game::Players& players) const
{
    if (get_num_dark_fields() >= get_num_light_fields()) {
        return {players.get_dark_player(), players.get_light_player()};
    }
    else {
        return {players.get_light_player(), players.get_dark_player()};
    }
}

} // namespace reviser
