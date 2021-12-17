#include "console_notifier.hpp"

#include <iostream>

namespace othellite::game {

void ConsoleNotifier::display_message(std::string_view const message)
{
    std::cout << message << "\n";
}

}
