#pragma once

#ifndef TEST_UTILITIES_HPP
#define TEST_UTILITIES_HPP

#include <sstream>
#include <string_view>

#include "board.hpp"
#include "game.hpp"
#include "player.hpp"
#include "position.hpp"

namespace othellite {
struct ConstantPlayerStub final : public game::Player
{

    explicit ConstantPlayerStub(
        std::string_view const player_name = "Unnamed Player",
        PlayerColor const pc = PlayerColor::dark,
        grid::Position played_position = grid::Position{grid::Row{0}, grid::Column{0}})
        : played_position{played_position}
    {
        name = player_name;
        color = pc;
    }


    [[nodiscard]] virtual grid::Position pick_move(Board const& board) const override
    {
        return played_position;
    }

    grid::Position played_position;
};

struct NotifierSpy final : public game::Notifier
{
    virtual void display_message(std::string_view const message) override
    {
        stream << message << "\n";
    }

    [[nodiscard]] std::string output() const
    {
        return stream.str();
    }

    std::stringstream stream{};
};

} // namespace othellite


#endif