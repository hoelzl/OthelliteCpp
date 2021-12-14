// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "common.hpp"
#include "doctest.hpp"

using othellite::Field;
using othellite::field_for_player_color;
using othellite::field_is_empty;
using othellite::field_is_occupied;
using othellite::field_is_owned_by_opponent_of;
using othellite::field_is_owned_by_player;
using othellite::field_to_char;
using othellite::other_player_color;
using othellite::PlayerColor;


TEST_CASE("field_is_empty()")
{
    CHECK(field_is_empty(Field::empty));
    CHECK_FALSE(field_is_empty(Field::light));
    CHECK_FALSE(field_is_empty(Field::dark));
}

TEST_CASE("field_is_occupied()")
{
    CHECK(field_is_occupied(Field::light));
    CHECK(field_is_occupied(Field::dark));
    CHECK_FALSE(field_is_occupied(Field::empty));
}

TEST_CASE("field_for_player_color()")
{
    CHECK(field_for_player_color(PlayerColor::light) == Field::light);
    CHECK(field_for_player_color(PlayerColor::dark) == Field::dark);
}

TEST_CASE("field_is_owned_by_player()")
{
    CHECK(field_is_owned_by_player(Field::light, PlayerColor::light));
    CHECK_FALSE(field_is_owned_by_player(Field::dark, PlayerColor::light));
    CHECK_FALSE(field_is_owned_by_player(Field::empty, PlayerColor::light));

    CHECK(field_is_owned_by_player(Field::dark, PlayerColor::dark));
    CHECK_FALSE(field_is_owned_by_player(Field::light, PlayerColor::dark));
    CHECK_FALSE(field_is_owned_by_player(Field::empty, PlayerColor::dark));
}

TEST_CASE("field_is_owned_by_opponent_of()")
{
    CHECK(field_is_owned_by_opponent_of(Field::dark, PlayerColor::light));
    CHECK_FALSE(field_is_owned_by_opponent_of(Field::light, PlayerColor::light));
    CHECK_FALSE(field_is_owned_by_opponent_of(Field::empty, PlayerColor::light));

    CHECK(field_is_owned_by_opponent_of(Field::light, PlayerColor::dark));
    CHECK_FALSE(field_is_owned_by_opponent_of(Field::dark, PlayerColor::dark));
    CHECK_FALSE(field_is_owned_by_opponent_of(Field::empty, PlayerColor::dark));
}

TEST_CASE("field_to_char()")
{
    CHECK(field_to_char(Field::light) == 'O');
    CHECK(field_to_char(Field::dark) == '*');
    CHECK(field_to_char(Field::empty) == ' ');
}

TEST_CASE("other_player_color()")
{
    CHECK(other_player_color(PlayerColor::light) == PlayerColor::dark);
    CHECK(other_player_color(PlayerColor::dark) == PlayerColor::light);
}
