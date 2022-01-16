// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#include "board.hpp"

#include "direction.hpp"
#include "position.hpp"


namespace reviser {

const std::vector<Position>& all_board_positions()
{
    static auto result = std::vector<Position>{};
    if (result.empty()) {
        for (auto row = 0; row < 8; ++row) {
            for (auto col = 0; col < 8; ++col) {
                result.push_back(Position{Row{row}, Column{col}});
            }
        }
    }
    return result;
}

} // namespace reviser
