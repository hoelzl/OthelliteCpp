// Copyright (c) 2021-2022 Dr. Matthias Hölzl.

#pragma once

#ifndef REVISER_LIB_CONSOLE_NOTIFIER_HPP
#define REVISER_LIB_CONSOLE_NOTIFIER_HPP

#include "game.hpp"

namespace reviser_cli {

class ConsoleNotifier final : public reviser::Notifier
{
public:
    ConsoleNotifier() = default;
    void display_message(std::string_view message) override;
};
} // namespace reviser_cli

#endif
