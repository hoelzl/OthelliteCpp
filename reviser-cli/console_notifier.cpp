// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "console_notifier.hpp"

#include <iostream>

namespace reviser_cli {

void ConsoleNotifier::display_message(const std::string_view message)
{
    std::cout << message << "\n";
}

} // namespace reviser_cli
