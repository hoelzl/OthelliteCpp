// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "common.hpp"

#include "doctest.hpp"
#include "utilities.hpp"

using namespace reviser;
using namespace std::string_literals;

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

TEST_CASE("player_color_to_string()")
{
    CHECK(player_color_to_string(PlayerColor::dark) == "dark"s);
    CHECK(player_color_to_string(PlayerColor::light) == "light"s);
}

TEST_CASE("class Score")
{
    auto score = Score{10, 20, 34};

    SUBCASE("Simple getters")
    {
        CHECK(score.get_num_dark_fields() == 10);
        CHECK(score.get_num_light_fields() == 20);
        CHECK(score.get_num_empty_fields() == 34);
    }

    SUBCASE("get_num_fields_for(PlayerColor)")
    {
        CHECK(score.get_num_fields_for(PlayerColor::dark) == 10);
        CHECK(score.get_num_fields_for(PlayerColor::light) == 20);
    }

    SUBCASE("get_num_fields_for(Player)")
    {
        auto dark_player = ConstantPlayerStub{"dark_player", PlayerColor::dark};
        auto light_player = ConstantPlayerStub{"light_player", PlayerColor::light};

        CHECK(score.get_num_fields_for(dark_player) == 10);
        CHECK(score.get_num_fields_for(light_player) == 20);
    }

    SUBCASE("to_string()")
    {
        CHECK(score.to_string(PlayerColor::dark) == "10:20"s);
        CHECK(score.to_string(PlayerColor::light) == "20:10"s);
        CHECK(
            score.to_string(ConstantPlayerStub{"player", PlayerColor::dark})
            == "10:20"s);
        CHECK(
            score.to_string(ConstantPlayerStub{"player", PlayerColor::light})
            == "20:10"s);
    }
}
