// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "position.hpp"

#include <string>


std::ostream&
othellite::grid::operator<<(std::ostream& os, othellite::grid::Position const pos)
{
    os << "Position(" << std::to_string(pos.get_row().value) << ", "
       << std::to_string(pos.get_column().value) << ")";
    return os;
}
