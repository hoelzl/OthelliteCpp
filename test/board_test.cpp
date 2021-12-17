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
    auto const board_str = "|O|*| | |O|*| | |\n"
                           "|O|*| | |O|*| |*|\n"
                           "|O|*| | |O|*| | |\n"
                           "|O|*| | |O|*| | |\n"
                           "|O|*| | |O|*| | |\n"
                           "|O|*| | |O|*| | |\n"
                           "|O|*| | |O|*| | |\n"
                           "|O|*| | |O|*| |O|";
    CHECK(Board::from_string(board_str).to_string() == std::string{board_str});
}

TEST_CASE("Board::initialize()")
{
    auto board_str = "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| |*|\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| | |\n"
                     "|O|*| | |O|*| |O|";
    auto board = Board::from_string(board_str);

    SUBCASE("InitialState::empty sets all fields to empty")
    {
        board.initialize(Board::InitialState::empty);
        for (auto pos : Board::get_positions()) {
            CHECK(board[pos] == Field::empty);
        }
    }

    SUBCASE("InitialState::center_square sets all fields, center fields are occupied")
    {
        auto light_fields = std::set<Position>{
            Position{Row{3}, Column{4}}, Position{Row{4}, Column{3}}};
        auto dark_fields = std::set<Position>{
            Position{Row{3}, Column{3}}, Position{Row{4}, Column{4}}};
        board.initialize(Board::InitialState::center_square);
        for (auto pos : Board::get_positions()) {
            if (light_fields.contains(pos)) {
                CHECK(board[pos] == Field::light);
            }
            else if (dark_fields.contains(pos)) {
                CHECK(board[pos] == Field::dark);
            }
            else {
                CHECK(board[pos] == Field::empty);
            }
        }
    }
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
                    CHECK(board.is_valid_move(
                        PlayerColor::light, Position{Row{row}, Column{col}}));
                }
                else {
                    CHECK_FALSE(board.is_valid_move(
                        PlayerColor::light, Position{Row{row}, Column{col}}));
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
                    CHECK(board.is_valid_move(
                        PlayerColor::dark, Position{Row{row}, Column{col}}));
                }
                else {
                    CHECK_FALSE(board.is_valid_move(
                        PlayerColor::dark, Position{Row{row}, Column{col}}));
                }
            }
        }
    }
}

void check_valid_moves(
    Board const& board, PlayerColor pc, std::set<Position> const& valid_moves)
{
    for (auto pos : Board::get_positions()) {
        auto result = board.is_valid_move(pc, pos);
        if (valid_moves.contains(pos)) {
            CHECK(result);
        }
        else {
            CHECK_FALSE(result);
        }
    }
}

TEST_CASE("Board::is_valid_moves() for initial board (alternative test "
          "implementation).")
{
    auto board = Board{};
    board.initialize(Board::InitialState::center_square);

    SUBCASE("Light has four moves.")
    {
        auto const valid_moves = std::set<Position>{
            Position{Row{2}, Column{3}},
            Position{Row{3}, Column{2}},
            Position{Row{4}, Column{5}},
            Position{Row{5}, Column{4}}};
        check_valid_moves(board, PlayerColor::light, valid_moves);
    }

    SUBCASE("Dark has four moves.")
    {
        auto const valid_moves = std::set<Position>{
            Position{Row{2}, Column{4}},
            Position{Row{3}, Column{5}},
            Position{Row{4}, Column{2}},
            Position{Row{5}, Column{3}}};
        check_valid_moves(board, PlayerColor::dark, valid_moves);
    }
}

TEST_CASE("Board::find_valid_moves() against initial board.")
{
    auto board = Board{};
    board.initialize(Board::InitialState::center_square);

    SUBCASE("Light has four moves.")
    {
        auto const valid_moves = std::set<Position>{
            Position{Row{2}, Column{3}},
            Position{Row{3}, Column{2}},
            Position{Row{4}, Column{5}},
            Position{Row{5}, Column{4}}};
        CHECK(board.find_valid_moves(PlayerColor::light) == valid_moves);
    }

    SUBCASE("Dark has four moves.")
    {
        auto const valid_moves = std::set<Position>{
            Position{Row{2}, Column{4}},
            Position{Row{3}, Column{5}},
            Position{Row{4}, Column{2}},
            Position{Row{5}, Column{3}}};
        CHECK(board.find_valid_moves(PlayerColor::dark) == valid_moves);
    }
}


TEST_CASE("Board::find_valid_moves() against board with occupied corner.")
{
    auto const board_str = "|*|O|O|O| | | | |\n"
                           "| |*| | | | | | |\n"
                           "| | | | | | | | |\n"
                           "| | | |*|O| | | |\n"
                           "| | | |O|*| | | |\n"
                           "| | | | | | | | |\n"
                           "| | | | | | | | |\n"
                           "| | | | | | | | |";
    auto const board = Board::from_string(board_str);

    SUBCASE("Light has six moves.")
    {
        auto const valid_moves = std::set<Position>{
            Position{Row{2}, Column{0}},
            Position{Row{2}, Column{1}},
            Position{Row{2}, Column{3}},
            Position{Row{3}, Column{2}},
            Position{Row{4}, Column{5}},
            Position{Row{5}, Column{4}}};
        CHECK(board.find_valid_moves(PlayerColor::light) == valid_moves);
    }

    SUBCASE("Dark has five moves.")
    {
        auto const valid_moves = std::set<Position>{
            Position{Row{0}, Column{4}},
            Position{Row{2}, Column{4}},
            Position{Row{3}, Column{5}},
            Position{Row{4}, Column{2}},
            Position{Row{5}, Column{3}}};
        CHECK(board.find_valid_moves(PlayerColor::dark) == valid_moves);
    }
}

TEST_CASE("Board::play_move()")
{
    auto board = Board::from_string("|*|O|O|O| | | | |\n"
                                    "| |*| | | | | | |\n"
                                    "| | | | | | | | |\n"
                                    "| | | |*|O| | | |\n"
                                    "| | | |O|*| | | |\n"
                                    "| | | | | | | | |\n"
                                    "| | | | | | | | |\n"
                                    "| | | | | | | | |");

    SUBCASE("Light player moves.")
    {
        SUBCASE("Light player plays (2, 0).")
        {
            board.play_move(PlayerColor::light, Position{Row{2}, Column{0}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |O| | | | | | |\n"
                                               "|O| | | | | | | |\n"
                                               "| | | |*|O| | | |\n"
                                               "| | | |O|*| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }

        SUBCASE("Light player plays (2, 1).")
        {
            board.play_move(PlayerColor::light, Position{Row{2}, Column{1}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |O| | | | | | |\n"
                                               "| |O| | | | | | |\n"
                                               "| | | |*|O| | | |\n"
                                               "| | | |O|*| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }

        SUBCASE("Light player plays (2, 3).")
        {
            board.play_move(PlayerColor::light, Position{Row{2}, Column{3}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | |O| | | | |\n"
                                               "| | | |O|O| | | |\n"
                                               "| | | |O|*| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }

        SUBCASE("Light player plays (3, 2).")
        {
            board.play_move(PlayerColor::light, Position{Row{3}, Column{2}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | |O|O|O| | | |\n"
                                               "| | | |O|*| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }

        SUBCASE("Light player plays (4, 5).")
        {
            board.play_move(PlayerColor::light, Position{Row{4}, Column{5}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | |*|O| | | |\n"
                                               "| | | |O|O|O| | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }

        SUBCASE("Light player plays (5, 4).")
        {
            board.play_move(PlayerColor::light, Position{Row{5}, Column{4}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | |*|O| | | |\n"
                                               "| | | |O|O| | | |\n"
                                               "| | | | |O| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }
    }

    SUBCASE("Dark player moves.")
    {
        SUBCASE("Dark player plays (0, 4).")
        {
            board.play_move(PlayerColor::dark, Position{Row{0}, Column{4}});

            auto expected = Board::from_string("|*|*|*|*|*| | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | |*|O| | | |\n"
                                               "| | | |O|*| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }
        SUBCASE("Dark player plays (2, 4).")
        {
            board.play_move(PlayerColor::dark, Position{Row{2}, Column{4}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | | |*| | | |\n"
                                               "| | | |*|*| | | |\n"
                                               "| | | |O|*| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }
        SUBCASE("Dark player plays (3, 5).")
        {
            board.play_move(PlayerColor::dark, Position{Row{3}, Column{5}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | |*|*|*| | |\n"
                                               "| | | |O|*| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }
        SUBCASE("Dark player plays (4, 2).")
        {
            board.play_move(PlayerColor::dark, Position{Row{4}, Column{2}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | |*|O| | | |\n"
                                               "| | |*|*|*| | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }
        SUBCASE("Dark player plays (5, 3).")
        {
            board.play_move(PlayerColor::dark, Position{Row{5}, Column{3}});

            auto expected = Board::from_string("|*|O|O|O| | | | |\n"
                                               "| |*| | | | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | |*|O| | | |\n"
                                               "| | | |*|*| | | |\n"
                                               "| | | |*| | | | |\n"
                                               "| | | | | | | | |\n"
                                               "| | | | | | | | |");
            CHECK(board == expected);
        }
    }
}


TEST_CASE("BoardReader::clean_board_str()")
{
    std::string const input{"a *bcd*0O*O !!!!!  O"};
    std::string const expected{" **O*O   O"};
    CHECK(BoardReader::clean_board_str(input) == expected);
}

TEST_CASE("BoardReader::convert_char()")
{
    CHECK(BoardReader::convert_char('O') == Field::light);
    CHECK(BoardReader::convert_char('*') == Field::dark);
    CHECK(BoardReader::convert_char(' ') == Field::empty);
    CHECK_THROWS_AS(BoardReader::convert_char('a'), std::invalid_argument);
}
