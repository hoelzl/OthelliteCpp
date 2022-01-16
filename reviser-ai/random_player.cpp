#include "random_player.hpp"

#include <cassert>

namespace reviser::ai {

auto RandomPlayer::pick_move(const ArrayBoard& board) const -> Position
{
    const auto moves = board.find_valid_moves(get_color());

    std::vector<Position> out{};
    std::ranges::sample(moves, std::back_inserter(out), 1, make_rng());

    assert(!out.empty());
    return out[0];
}

} // namespace reviser::ai
