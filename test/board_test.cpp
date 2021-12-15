// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "board.hpp"
#include "doctest.hpp"

using othellite::Board;
using othellite::BoardReader;
using othellite::Field;
using othellite::PlayerColor;
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

TEST_CASE("Board::is_valid_move()")
{
    auto board_str = "| | | | | | | | |\n"
                     "| | | | | | | | |\n"
                     "| | | | | | | | |\n"
                     "| | | |*|O| | | |\n"
                     "| | | |O|*| | | |\n"
                     "| | | | | | | | |\n"
                     "| | | | | | | | |\n"
                     "| | | | | | | | |";
    auto board = Board::from_string(board_str);

    SUBCASE("All light moves are classified correctly.")
    {
        std::set<std::tuple<int, int>> light_moves = {{2, 3}, {3, 2}, {4, 5}, {5, 4}};
        for (auto row = 0; row < 8; ++row) {
            for (auto col = 0; col < 8; ++col) {
                if (light_moves.contains({row, col})) {
                    CHECK(board.is_valid_move(PlayerColor::light,
                                              Position{Row{row}, Column{col}}));
                } else {
                    CHECK_FALSE(board.is_valid_move(PlayerColor::light,
                                                    Position{Row{row}, Column{col}}));
                }
            }
        }
    }

    SUBCASE("All dark moves are classified correctly.")
    {
        std::set<std::tuple<int, int>> light_moves = {{2, 4}, {3, 5}, {4, 2}, {5, 3}};
        for (auto row = 0; row < 8; ++row) {
            for (auto col = 0; col < 8; ++col) {
                if (light_moves.contains({row, col})) {
                    CHECK(board.is_valid_move(PlayerColor::dark,
                                              Position{Row{row}, Column{col}}));
                } else {
                    CHECK_FALSE(board.is_valid_move(PlayerColor::dark,
                                                    Position{Row{row}, Column{col}}));
                }
            }
        }
    }
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
