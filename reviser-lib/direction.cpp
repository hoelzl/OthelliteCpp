// Copyright (c) 2021 Dr. Matthias Hölzl.
#include "direction.hpp"

#include <ostream>

std::ostream&
reviser::grid::operator<<(std::ostream& os, const VerticalMovement vertical_movement)
{
    const auto amount = vertical_movement.get_amount();
    os << abs(amount) << (amount <= 0 ? "N" : "S");
    return os;
}

std::ostream& reviser::grid::operator<<(
    std::ostream& os, const HorizontalMovement horizontal_movement)
{
    const auto amount = horizontal_movement.get_amount();
    os << abs(amount) << (amount < 0 ? "W" : "E");
    return os;
}

std::ostream& reviser::grid::operator<<(std::ostream& os, const Direction d)
{
    os << "Direction(" << d.get_dy() << ", " << d.get_dx() << ")";
    return os;
}
