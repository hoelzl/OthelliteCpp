// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "position.hpp"

#include <sstream>
#include <stdexcept>

#include "doctest.hpp"

using namespace reviser::grid;

TEST_CASE("Rows with valid indices can be created and used.")
{
    SUBCASE("Rows can be constructed from integers.") { CHECK_NOTHROW(Row{1}); }

    SUBCASE("Rows are automatically converted to integers.")
    {
        auto constexpr row = Row{1};
        CHECK(row + 1 == 2);
    }
}

TEST_CASE("Rows can be compared for equality.")
{
    auto constexpr row1 = Row{3};
    auto constexpr row2 = Row{3};
    auto constexpr row3 = Row{2};

    CHECK(row1 == row2);
    CHECK(row2 != row3);
    CHECK(row1 != row3);
}

TEST_CASE("Columns can be compared for equality.")
{
    auto constexpr column1 = Column{3};
    auto constexpr column2 = Column{3};
    auto constexpr column3 = Column{2};

    CHECK(column1 == column2);
    CHECK(column2 != column3);
    CHECK(column1 != column3);
}

TEST_CASE("Columns with valid indices can be created and used.")
{
    SUBCASE("Columns can be constructed from integers.") { CHECK_NOTHROW(Column{1}); }

    SUBCASE("Columns are automatically converted to integers.")
    {
        auto constexpr column = Column{1};
        CHECK(column + 1 == 2);
    }
}

TEST_CASE("Positions can be constructed.")
{
    CHECK_NOTHROW(Position{Row{1}, Column{2}});
}

TEST_CASE("Valid positions return correct values for simple operations.")
{
    auto constexpr pos = Position{Row{2}, Column{4}};

    SUBCASE("get_row() and get_column() return correct values.")
    {
        CHECK(pos.get_row() == Row{2});
        CHECK(pos.get_column() == Column{4});
    }

    SUBCASE("is_valid() returns correct value.") { CHECK(pos.is_valid() == true); }
}

TEST_CASE("Positions can be compared for equality.")
{
    auto pos1 = Position{Row{7}, Column{1}};
    auto pos2 = Position{Row{7}, Column{1}};
    auto pos3 = Position{Row{4}, Column{1}};
    auto pos4 = Position{Row{7}, Column{2}};

    CHECK(pos1 == pos2);
    CHECK(pos2 != pos3);
    CHECK(pos2 != pos3);
    CHECK(pos3 != pos4);
}

TEST_CASE("is_valid() returns false for invalid positions")
{
    auto pos = Position{Row{-1}, Column{2}};
    CHECK(pos.is_valid() == false);
    pos = Position{Row{8}, Column{2}};
    CHECK(pos.is_valid() == false);
    pos = Position{Row{6}, Column{-1}};
    CHECK(pos.is_valid() == false);
    pos = Position{Row{6}, Column{8}};
    CHECK(pos.is_valid() == false);
}

TEST_CASE("Next position is computed correctly when inside board dimensions.")
{
    auto pos = Position{Row{3}, Column{6}};

    SUBCASE("next_in_direction() works for major compass directions.")
    {
        auto next_pos = pos.next_in_direction(N);
        CHECK(next_pos == Position{Row{2}, Column{6}});
        next_pos = pos.next_in_direction(S);
        CHECK(next_pos == Position{Row{4}, Column{6}});
        next_pos = pos.next_in_direction(W);
        CHECK(next_pos == Position{Row{3}, Column{5}});
        next_pos = pos.next_in_direction(E);
        CHECK(next_pos == Position{Row{3}, Column{7}});
    }
    SUBCASE("next_in_direction() works for major compass directions.")
    {
        auto next_pos = pos.next_in_direction(NW);
        CHECK(next_pos == Position{Row{2}, Column{5}});
        next_pos = pos.next_in_direction(NE);
        CHECK(next_pos == Position{Row{2}, Column{7}});
        next_pos = pos.next_in_direction(SW);
        CHECK(next_pos == Position{Row{4}, Column{5}});
        next_pos = pos.next_in_direction(SE);
        CHECK(next_pos == Position{Row{4}, Column{7}});
    }
}

TEST_CASE("next_in_direction() throws for invalid positions")
{
    auto pos = Position{Row{-1}, Column{2}};
    CHECK_THROWS_AS((void)pos.next_in_direction(N), std::invalid_argument);
    pos = Position{Row{8}, Column{2}};
    CHECK_THROWS_AS((void)pos.next_in_direction(N), std::invalid_argument);
    pos = Position{Row{6}, Column{-1}};
    CHECK_THROWS_AS((void)pos.next_in_direction(N), std::invalid_argument);
    pos = Position{Row{6}, Column{8}};
    CHECK_THROWS_AS((void)pos.next_in_direction(N), std::invalid_argument);
}

TEST_CASE("Linear indices are computed correctly.")
{
    SUBCASE("Linear indices for first row are between 0 and 7.")
    {
        auto pos = Position{Row{0}, Column{0}};
        CHECK(pos.to_linear_index() == 0);
        pos = Position{Row{0}, Column{1}};
        CHECK(pos.to_linear_index() == 1);
        pos = Position{Row{0}, Column{7}};
        CHECK(pos.to_linear_index() == 7);
    }

    SUBCASE("Linear indices for second row are between 8 and 15.")
    {
        auto pos = Position{Row{1}, Column{0}};
        CHECK(pos.to_linear_index() == 8);
        pos = Position{Row{1}, Column{1}};
        CHECK(pos.to_linear_index() == 9);
        pos = Position{Row{1}, Column{7}};
        CHECK(pos.to_linear_index() == 15);
    }

    SUBCASE("Linear indices for last row are between 56 and 63.")
    {
        auto pos = Position{Row{7}, Column{0}};
        CHECK(pos.to_linear_index() == 56);
        pos = Position{Row{7}, Column{1}};
        CHECK(pos.to_linear_index() == 57);
        pos = Position{Row{7}, Column{7}};
        CHECK(pos.to_linear_index() == 63);
    }
}

TEST_CASE("Comparison operators for positions work as expected.")
{
    auto p1 = Position{Row{3}, Column{7}};
    auto p2 = Position{Row{3}, Column{7}};
    auto p3 = Position{Row{3}, Column{8}};
    auto p4 = Position{Row{4}, Column{1}};
    auto p5 = Position{Row{5}, Column{7}};

    SUBCASE("operator==() returns true iff lhs == rhs.")
    {
        CHECK(p1 == p1);
        CHECK(p1 == p2);
        CHECK_FALSE(p1 == p3);
        CHECK_FALSE(p1 == p3);
        CHECK_FALSE(p1 == p5);
    }

    SUBCASE("operator!=() returns true iff lhs != rhs.")
    {
        CHECK_FALSE(p1 != p1);
        CHECK_FALSE(p1 != p2);
        CHECK(p1 != p3);
        CHECK(p1 != p3);
        CHECK(p1 != p5);
    }

    SUBCASE("operator<=() returns true iff lhs <= rhs.")
    {
        CHECK(p1 <= p1);
        CHECK(p1 <= p2);
        CHECK(p1 <= p3);
        CHECK(p1 <= p4);
        CHECK(p1 <= p5);
        CHECK(p2 <= p1);
        CHECK(p2 <= p2);
        CHECK(p2 <= p3);
        CHECK(p2 <= p4);
        CHECK(p2 <= p5);
        CHECK(p3 <= p3);
        CHECK(p3 <= p4);
        CHECK(p3 <= p5);
        CHECK(p4 <= p4);
        CHECK(p4 <= p5);
        CHECK(p5 <= p5);
    }

    SUBCASE("operator<=() returns false iff lhs > rhs.")
    {
        CHECK_FALSE(p3 <= p1);
        CHECK_FALSE(p3 <= p2);
        CHECK_FALSE(p4 <= p1);
        CHECK_FALSE(p4 <= p2);
        CHECK_FALSE(p4 <= p3);
        CHECK_FALSE(p5 <= p1);
        CHECK_FALSE(p5 <= p2);
        CHECK_FALSE(p5 <= p3);
        CHECK_FALSE(p5 <= p4);
    }

    SUBCASE("operator<() returns true iff lhs < rhs.")
    {
        CHECK(p1 < p3);
        CHECK(p1 < p4);
        CHECK(p1 < p5);
        CHECK(p2 < p3);
        CHECK(p2 < p4);
        CHECK(p2 < p5);
        CHECK(p3 < p4);
        CHECK(p3 < p5);
        CHECK(p4 < p5);
    }

    SUBCASE("operator<() returns false iff lhs >= rhs.")
    {
        CHECK_FALSE(p1 < p1);
        CHECK_FALSE(p1 < p2);
        CHECK_FALSE(p2 < p1);
        CHECK_FALSE(p2 < p2);
        CHECK_FALSE(p3 < p1);
        CHECK_FALSE(p3 < p2);
        CHECK_FALSE(p3 < p3);
        CHECK_FALSE(p4 < p1);
        CHECK_FALSE(p4 < p2);
        CHECK_FALSE(p4 < p3);
        CHECK_FALSE(p4 < p4);
        CHECK_FALSE(p5 < p1);
        CHECK_FALSE(p5 < p2);
        CHECK_FALSE(p5 < p3);
        CHECK_FALSE(p5 < p4);
        CHECK_FALSE(p5 < p5);
    }
}

TEST_CASE("Positions are formatted correctly.")
{
    auto constexpr pos = Position{Row{2}, Column{5}};
    std::stringstream os{};
    os << pos;

    CHECK(os.str() == "Position(2, 5)");
}
