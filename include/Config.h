#pragma once

#include <cstdint>

struct Config
{
    static constexpr uint8_t ledPin = 4;
    static constexpr uint8_t buttonPin = 5;

    static constexpr uint32_t blinkIntervalMs = 300;
    static constexpr uint32_t debounceDelayMs = 50;

    static constexpr uint32_t serialBaudRate = 115200;
};