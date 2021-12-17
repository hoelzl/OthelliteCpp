#pragma once

#ifndef OTHELLITE_LIB_CONSOLE_NOTIFIER_HPP
#define OTHELLITE_LIB_CONSOLE_NOTIFIER_HPP

#include "game.hpp"

namespace othellite::game {

class ConsoleNotifier final : public Notifier
{
public:
    ConsoleNotifier() = default;
    virtual void display_message(std::string_view message) override;
};
}

#endif