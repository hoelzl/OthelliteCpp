// Copyright (c) 2021 Dr. Matthias Hölzl.

#include "common.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>

namespace othellite::grid {

class Movement
{
public:
    using int_type = int_least8_t;

    constexpr operator int_least8_t() const // NOLINT(google-explicit-constructor)
    {
        return amount;
    }

    [[nodiscard]] Movement::int_type get_amount() const { return amount; }

protected:
    explicit constexpr Movement(int_type amount) : amount{amount} {}
    int_type amount;
};

class HorizontalMovement : public Movement
{
    explicit constexpr HorizontalMovement(Movement::int_type amount) : Movement(amount)
    {
    }
    friend consteval class HorizontalMovement operator"" _W(unsigned long long amount);
    friend consteval class HorizontalMovement operator"" _E(unsigned long long amount);
    friend std::ostream& operator<<(std::ostream& os,
                                    HorizontalMovement horizontal_movement);
};

class VerticalMovement : public Movement
{
    explicit constexpr VerticalMovement(Movement::int_type amount) : Movement(amount) {}
    friend consteval class VerticalMovement operator"" _N(unsigned long long amount);
    friend consteval class VerticalMovement operator"" _S(unsigned long long amount);
    friend std::ostream& operator<<(std::ostream& os,
                                    VerticalMovement vertical_movement);
};

consteval HorizontalMovement operator"" _W(unsigned long long amount)
{
    if (amount < std::numeric_limits<Movement::int_type>::max()) {
        return HorizontalMovement{static_cast<Movement::int_type>(-1 * amount)};
    }
    throw std::invalid_argument("Bad argument for _W literal.");
}

consteval HorizontalMovement operator"" _E(unsigned long long amount)
{
    if (amount < std::numeric_limits<Movement::int_type>::max()) {
        return HorizontalMovement{static_cast<Movement::int_type>(amount)};
    }
    throw std::invalid_argument("Bad argument for _E literal.");
}

std::ostream& operator<<(std::ostream& os, HorizontalMovement horizontal_movement);

consteval VerticalMovement operator"" _N(unsigned long long amount)
{
    if (amount < std::numeric_limits<Movement::int_type>::max()) {
        return VerticalMovement{static_cast<Movement::int_type>(-1 * amount)};
    }
    throw std::invalid_argument("Bad argument for _N literal.");
}

consteval VerticalMovement operator"" _S(unsigned long long amount)
{
    if (amount < std::numeric_limits<Movement::int_type>::max()) {
        return VerticalMovement{static_cast<Movement::int_type>(amount)};
    }
    throw std::invalid_argument("Bad argument for _S literal.");
}

std::ostream& operator<<(std::ostream& os, VerticalMovement vertical_movement);

class Direction
{
public:
    constexpr Direction(VerticalMovement dy, HorizontalMovement dx) : dy{dy}, dx{dx} {}
    constexpr ~Direction() = default;

    [[nodiscard]] constexpr HorizontalMovement get_dx() const { return dx; }
    [[nodiscard]] constexpr VerticalMovement get_dy() const { return dy; }

private:
    HorizontalMovement dx;
    VerticalMovement dy;
};

std::ostream& operator<<(std::ostream& os, Direction d);

constexpr Direction N{1_N, 0_E};
constexpr Direction NE{1_N, 1_E};
constexpr Direction E{0_N, 1_E};
constexpr Direction SE{1_S, 1_E};
constexpr Direction S{1_S, 0_E};
constexpr Direction SW{1_S, 1_W};
constexpr Direction W{0_N, 1_W};
constexpr Direction NW{1_N, 1_W};

} // namespace othellite::grid
