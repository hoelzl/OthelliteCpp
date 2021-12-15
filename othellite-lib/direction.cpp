// Copyright (c) 2021 Dr. Matthias Hölzl.
#include "direction.hpp"

#include <ostream>

std::ostream&
othellite::grid::operator<<(std::ostream& os, VerticalMovement const vertical_movement)
{
    auto const amount = vertical_movement.get_amount();
    os << abs(amount) << (amount <= 0 ? "N" : "S");
    return os;
}

std::ostream& othellite::grid::operator<<(
    std::ostream& os, HorizontalMovement const horizontal_movement)
{
    auto const amount = horizontal_movement.get_amount();
    os << abs(amount) << (amount < 0 ? "W" : "E");
    return os;
}

std::ostream& othellite::grid::operator<<(std::ostream& os, Direction const d)
{
    os << "Direction(" << d.get_dy() << ", " << d.get_dx() << ")";
    return os;
}
