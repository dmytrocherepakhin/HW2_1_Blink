#pragma once

#include <cstdint>


enum class LedState : uint8_t
{
    Off,
    On
};


class Led
{
public:
    explicit Led(uint8_t pin);

    void init();
    void set(LedState state);
    void toggle();

    LedState state() const;

private:
    uint8_t pin_;
    LedState state_ = LedState::Off;
};