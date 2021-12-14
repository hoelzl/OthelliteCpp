// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "board.hpp"
#include "doctest.hpp"

using othellite::Board;
using othellite::BoardReader;
using othellite::Field;
using othellite::grid::Column;
using othellite::grid::Position;
using othellite::grid::Row;

TEST_CASE("Board::from_string()")
{
    auto board_str = "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| |*|\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| |O|\n";

    SUBCASE("does not throw") { CHECK_NOTHROW(Board::from_string(board_str)); }

    SUBCASE("produces correct board")
    {
        auto board = Board::from_string(board_str);
        CHECK(board[Position{Row{0}, Column{0}}] == Field::light);
        CHECK(board[Position{Row{0}, Column{1}}] == Field::dark);
        CHECK(board[Position{Row{0}, Column{2}}] == Field::empty);
        CHECK(board[Position{Row{0}, Column{3}}] == Field::empty);
        CHECK(board[Position{Row{0}, Column{7}}] == Field::empty);

        CHECK(board[Position{Row{1}, Column{0}}] == Field::light);
        CHECK(board[Position{Row{1}, Column{1}}] == Field::dark);
        CHECK(board[Position{Row{1}, Column{2}}] == Field::empty);
        CHECK(board[Position{Row{1}, Column{3}}] == Field::empty);
        CHECK(board[Position{Row{1}, Column{7}}] == Field::dark);

        CHECK(board[Position{Row{7}, Column{0}}] == Field::light);
        CHECK(board[Position{Row{7}, Column{1}}] == Field::dark);
        CHECK(board[Position{Row{7}, Column{2}}] == Field::empty);
        CHECK(board[Position{Row{7}, Column{3}}] == Field::empty);
        CHECK(board[Position{Row{7}, Column{7}}] == Field::light);
    }
}

TEST_CASE("Board::to_string()")
{
    auto board_str = "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| |*|\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| |O|";
    CHECK(Board::from_string(board_str).to_string() == std::string{board_str});
}

TEST_CASE("BoardReader::clean_board_str()")
{
    std::string input{"a *bcd*0O*O !!!!!  O"};
    std::string expected{" **O*O   O"};
    CHECK(BoardReader::clean_board_str(input) == expected);
}

TEST_CASE("BoardReader::convert_char()")
{
    CHECK(BoardReader::convert_char('O') == Field::light);
    CHECK(BoardReader::convert_char('*') == Field::dark);
    CHECK(BoardReader::convert_char(' ') == Field::empty);
    CHECK_THROWS_AS(BoardReader::convert_char('a'), std::invalid_argument);
}
