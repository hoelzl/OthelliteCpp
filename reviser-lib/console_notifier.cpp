// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "console_notifier.hpp"

#include <iostream>

namespace reviser::game {

void ConsoleNotifier::display_message(std::string_view const message)
{
    std::cout << message << "\n";
}

} // namespace reviser::game
