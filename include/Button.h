#pragma once

#include <Arduino.h>
#include <cstdint>

class Button
{
public:
    explicit Button(uint8_t pin, uint32_t debounceDelayMs);

    void init();
    bool wasPressed(uint32_t now);

private:
    uint8_t pin_;
    uint32_t debounceDelayMs_;

    uint8_t lastReading_ = HIGH;
    uint8_t stableReading_ = HIGH;

    uint32_t lastChangeTime_ = 0;
};