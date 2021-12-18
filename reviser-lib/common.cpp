// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "common.hpp"

#include <format>
#include <stdexcept>

#include "game.hpp"
#include "player.hpp"

namespace reviser {

bool field_is_empty(Field const field) { return field == Field::empty; }

bool field_is_occupied(Field const field) { return field != Field::empty; }

auto field_for_player_color(PlayerColor const pc) -> Field
{
    switch (pc) {
    case PlayerColor::light: return Field::light;
    case PlayerColor::dark: return Field::dark;
    }
    throw std::invalid_argument("Unexpected value for player color.");
}

bool field_is_owned_by_player(Field const field, PlayerColor const pc)
{
    return field == field_for_player_color(pc);
}

bool field_is_owned_by_opponent_of(Field const field, PlayerColor const pc)
{
    return field_is_occupied(field) && !field_is_owned_by_player(field, pc);
}

char field_to_char(Field const field)
{
    switch (field) {
    case Field::light: return 'O';
    case Field::dark: return '*';
    case Field::empty: return ' ';
    }
    throw std::invalid_argument("Unexpected value for field.");
}

auto other_player_color(PlayerColor const pc) -> PlayerColor
{
    if (pc == PlayerColor::light) {
        return PlayerColor::dark;
    }
    else {
        return PlayerColor::light;
    }
}

std::string player_color_to_string(PlayerColor const pc)
{
    return pc == PlayerColor::dark ? "dark" : "light";
}

int_fast8_t Score::get_num_fields_for(PlayerColor const pc) const
{
    switch (pc) {
    case PlayerColor::dark: return num_dark_fields;
    case PlayerColor::light: return num_light_fields;
    }
    throw std::invalid_argument{"Bad player color."};
}

int_fast8_t Score::get_num_fields_for(game::Player const& player) const
{
    return get_num_fields_for(player.get_color());
}


std::string Score::to_string(PlayerColor const first_player) const
{
    return std::format(
        "{}:{}",
        get_num_fields_for(first_player),
        get_num_fields_for(other_player_color(first_player)));
}

std::string Score::to_string(game::Player const& player) const
{
    return to_string(player.get_color());
}

bool Score::is_tied() const { return get_num_dark_fields() == get_num_light_fields(); }

std::tuple<game::Player const&, game::Player const&>
Score::compute_winner(game::Players const& players) const
{
    if (get_num_dark_fields() >= get_num_light_fields()) {
        return {players.get_dark_player(), players.get_light_player()};
    }
    else {
        return {players.get_light_player(), players.get_dark_player()};
    }
}

} // namespace reviser
