// Copyright (c) 2021 Dr. Matthias Hölzl.
#include "direction.hpp"
#include <ostream>

std::ostream& othellite::grid::operator<<(std::ostream& os,
                                          VerticalMovement vertical_movement)
{
    auto amount = vertical_movement.get_amount();
    os << abs(amount) << (amount <= 0 ? "N" : "S");
    return os;
}

std::ostream& othellite::grid::operator<<(std::ostream& os,
                                          HorizontalMovement horizontal_movement)
{
    auto amount = horizontal_movement.get_amount();
    os << abs(amount) << (amount < 0 ? "W" : "E");
    return os;
}

std::ostream& othellite::grid::operator<<(std::ostream& os, Direction d)
{
    os << "Direction(" << d.get_dy() << ", " << d.get_dx() << ")";
    return os;
}
