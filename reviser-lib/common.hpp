// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once
#ifndef REVISER_LIB_COMMON_HPP
#define REVISER_LIB_COMMON_HPP

#include <cstdint>
#include <map>
#include <string>

namespace reviser {
class Players;
class Player;
} // namespace reviser

namespace reviser {

constexpr int_least8_t board_size{8};

enum class Field : int_least8_t
{
    empty,
    light,
    dark,
};

std::ostream& operator<<(std::ostream& os, Field field);

enum class PlayerColor : int_least8_t
{
    dark,
    light,
};

std::ostream& operator<<(std::ostream& os, PlayerColor pc);


bool field_is_empty(Field field);
bool field_is_occupied(Field field);

Field field_for_player_color(PlayerColor pc);
bool field_is_owned_by_player(Field field, PlayerColor pc);
bool field_is_owned_by_opponent_of(Field field, PlayerColor pc);

char field_to_char(Field field);

PlayerColor other_player_color(PlayerColor pc);
std::string player_color_to_string(PlayerColor pc);

template <typename K, std::integral V>
V find_default(const std::map<K, V>& values, K value)
{
    const auto it{values.find(value)};
    if (it == values.cend()) {
        return V{};
    }
    return it->second;
}

class Score
{
public:
    Score(
        const int_fast8_t num_dark_fields,
        const int_fast8_t num_light_fields,
        const int_fast8_t num_empty_fields)
        : num_dark_fields{num_dark_fields}
        , num_light_fields{num_light_fields}
        , num_empty_fields{num_empty_fields}
    {}

    explicit Score(const std::map<Field, int_fast8_t>& values)
        : Score{
            find_default(values, Field::dark),
            find_default(values, Field::light),
            find_default(values, Field::empty)}
    {}

    [[nodiscard]] int_fast8_t get_num_dark_fields() const { return num_dark_fields; }
    [[nodiscard]] int_fast8_t get_num_light_fields() const { return num_light_fields; }
    [[nodiscard]] int_fast8_t get_num_empty_fields() const { return num_empty_fields; }

    [[nodiscard]] int_fast8_t get_num_fields_for(PlayerColor pc) const;
    [[nodiscard]] int_fast8_t get_num_fields_for(const Player& player) const;

    [[nodiscard]] std::string to_string(PlayerColor first_player) const;
    [[nodiscard]] std::string to_string(const Player& player) const;

    [[nodiscard]] bool is_tied() const;
    [[nodiscard]] std::tuple<const Player&, const Player&>
    compute_winner(const Players& players) const;

private:
    int_fast8_t num_dark_fields{};
    int_fast8_t num_light_fields{};
    int_fast8_t num_empty_fields{};
};


} // namespace reviser


#endif // REVISER_LIB_COMMON_HPP
