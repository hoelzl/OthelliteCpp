#include "game.hpp"

#include <string>

#include "doctest.hpp"
#include "game_result.hpp"
#include "utilities.hpp"

using namespace std::string_literals;

namespace othellite {

TEST_CASE("Notifier")
{
    auto notifier = NotifierSpy{};

    SUBCASE("display_board()")
    {
        auto const board_string = "|*|O|O|O| | | | |\n"
                                  "| |*| | | | | | |\n"
                                  "| | | |O| | | | |\n"
                                  "| | | |O|O| | | |\n"
                                  "| | | |O|*| | | |\n"
                                  "| | | | | | | | |\n"
                                  "| |*|O|*|O|*|O| |\n"
                                  "| | |*|O|*|O| | |"s;
        auto const board = Board::from_string(board_string);

        notifier.display_board(board);
        CHECK(notifier.output() == board_string + "\n"s);
    }

    SUBCASE("note_new_game()")
    {
        auto dark_player = ConstantPlayerStub{"dark_player", PlayerColor::dark};
        auto light_player = ConstantPlayerStub{"light_player", PlayerColor::light};
        auto players = game::Players{dark_player, light_player};

        auto constexpr board = Board{};
        std::string expected =
            ("Starting a new game.\n"s + "Dark player: dark_player\n"s +
             "Light player: light_player\n"s);

        notifier.note_new_game(players, board);

        CHECK(notifier.output() == expected);
    }

    SUBCASE("note_new_game()")
    {
        auto dark_player = ConstantPlayerStub{"dark_player", PlayerColor::dark};

        auto board_string = "|*|O|O|O| | | | |\n"
                            "| |*| | | | | | |\n"
                            "| | | | | | | | |\n"
                            "| | | |*|O| | | |\n"
                            "| | | |O|*| | | |\n"
                            "| | | | | | | | |\n"
                            "| | | | | | | | |\n"
                            "| | | | | | | | |"s;
        auto board = Board::from_string(board_string);
        std::string expected =
            ("Player (dark_player) plays (1, 2).\n"s + board_string + "\n"s);

        notifier.note_move(
            dark_player, grid::Position{grid::Row{1}, grid::Column{2}}, board);

        CHECK(notifier.output().size() == expected.size());
        CHECK(notifier.output() == expected);
    }

    SUBCASE("note_result()")
    {
        auto dark_player = ConstantPlayerStub{"dark_player", PlayerColor::dark};
        auto light_player = ConstantPlayerStub{"light_player", PlayerColor::light};
        auto board = Board{};

        SUBCASE("Dark player won with best score.")
        {
            auto score = Score{44, 20, 0};
            auto result = game::WinByScore{score, board, dark_player, light_player};
            auto expected =
                "GAME OVER.\ndark_player (dark) won.\nThe score was 44:20.\n"s;

            notifier.note_result(result);

            CHECK(notifier.output() == expected);
        }

        SUBCASE("White player won with best score.")
        {
            auto score = Score{24, 40, 0};
            auto result = game::WinByScore{score, board, light_player, dark_player};
            auto expected =
                "GAME OVER.\nlight_player (light) won.\nThe score was 40:24.\n"s;

            notifier.note_result(result);

            CHECK(notifier.output() == expected);
        }

        SUBCASE("Dark player won because of error by light player.")
        {
            auto score = Score{24, 40, 0};
            auto result =
                game::WinByOpponentMistake{score, board, dark_player, light_player};
            auto expected =
                "GAME OVER.\ndark_player (dark) won.\nThe opponent made an invalid move.\n"s;

            notifier.note_result(result);

            CHECK(notifier.output() == expected);
        }

        SUBCASE("Tie.")
        {
            auto score = Score{32, 32, 0};
            auto result =
                game::TiedResult{score, board, dark_player, light_player};
            auto expected =
                "GAME OVER.\nThe game was a tie.\nThe score was 32:32.\n"s;

            notifier.note_result(result);

            CHECK(notifier.output() == expected);
        }
    }
}
} // namespace othellite