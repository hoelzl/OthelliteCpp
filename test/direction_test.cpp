// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "direction.hpp"

#include <sstream>

#include "doctest.hpp"

using namespace reviser::grid;

TEST_CASE("HorizontalMovement can be constructed as literals")
{
    HorizontalMovement movement{12_W};
    CHECK(movement.get_amount() == -12);
    movement = 8_E;
    CHECK(movement.get_amount() == 8);
}

TEST_CASE("HorizontalMovement converts to integer.")
{
    auto movement{4_W};
    CHECK(movement == -4);
    movement = 5_E;
    CHECK(movement == 5);
}

TEST_CASE("HorizontalMovement is correctly written to stream.")
{
    auto os = std::stringstream{};
    os << 2_W << " " << 3_E;
    CHECK(os.str() == "2W 3E");
}

TEST_CASE("VerticalMovement can be constructed as literals")
{
    VerticalMovement movement{7_N};
    CHECK(movement.get_amount() == -7);
    movement = 3_S;
    CHECK(movement.get_amount() == 3);
}

TEST_CASE("HorizontalMovement converts to integer.")
{
    auto movement{7_N};
    CHECK(movement == -7);
    movement = 2_S;
    CHECK(movement == 2);
}

TEST_CASE("VerticalMovement is correctly written to stream.")
{
    auto os = std::stringstream{};
    os << 4_N << " " << 1_S;
    CHECK(os.str() == "4N 1S");
}

TEST_CASE("Direction can be constructed from movements.")
{
    auto d = Direction{1_N, 1_E};
    CHECK(d.get_dx() == 1_E);
    CHECK(d.get_dy() == 1_N);
}

TEST_CASE("Direction constants are correctly defined.")
{
    auto os = std::stringstream{};

    SUBCASE("Vertical main compass directions.")
    {
        os << N << " " << S;
        CHECK(os.str() == "Direction(1N, 0E) Direction(1S, 0E)");
    }

    SUBCASE("Horizontal main compass directions.")
    {
        os << W << " " << E;
        CHECK(os.str() == "Direction(0N, 1W) Direction(0N, 1E)");
    }
    SUBCASE("Northern mixed compass directions.")
    {
        os << NW << " " << NE;
        CHECK(os.str() == "Direction(1N, 1W) Direction(1N, 1E)");
    }

    SUBCASE("Southern mixed compass directions.")
    {
        os << SW << " " << SE;
        CHECK(os.str() == "Direction(1S, 1W) Direction(1S, 1E)");
    }
}
