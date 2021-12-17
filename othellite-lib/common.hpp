// Copyright (c) 2021 Dr. Matthias Hölzl.

#pragma once
#ifndef OTHELLITE_LIB_COMMON_HPP
#define OTHELLITE_LIB_COMMON_HPP

#include <cstdint>
#include <string>

namespace othellite::game {
class Players;
class Player;
} // namespace othellite::game

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
    dark,
    light,
};

bool field_is_empty(Field field);
bool field_is_occupied(Field field);

Field field_for_player_color(PlayerColor pc);
bool field_is_owned_by_player(Field field, PlayerColor pc);
bool field_is_owned_by_opponent_of(Field field, PlayerColor pc);

char field_to_char(Field field);

PlayerColor other_player_color(PlayerColor pc);
std::string player_color_to_string(PlayerColor pc);


class Score
{
public:
    Score(
        int_fast8_t const num_dark_fields,
        int_fast8_t const num_light_fields,
        int_fast8_t const num_empty_fields)
        : num_dark_fields{num_dark_fields}
        , num_light_fields{num_light_fields}
        , num_empty_fields{num_empty_fields}
    {}

    [[nodiscard]] int_fast8_t get_num_dark_fields() const { return num_dark_fields; }
    [[nodiscard]] int_fast8_t get_num_light_fields() const { return num_light_fields; }
    [[nodiscard]] int_fast8_t get_num_empty_fields() const { return num_empty_fields; }

    [[nodiscard]] int_fast8_t get_num_fields_for(PlayerColor pc) const;
    [[nodiscard]] int_fast8_t get_num_fields_for(game::Player const& player) const;

    [[nodiscard]] std::string to_string(PlayerColor first_player) const;
    [[nodiscard]] std::string to_string(game::Player const& player) const;

    [[nodiscard]] bool is_tied() const;
    [[nodiscard]] std::tuple<game::Player const&, game::Player const&>
    compute_winner(game::Players const& players) const;

private:
    int_fast8_t num_dark_fields{};
    int_fast8_t num_light_fields{};
    int_fast8_t num_empty_fields{};
};


} // namespace othellite


#endif // OTHELLITE_LIB_COMMON_HPP
