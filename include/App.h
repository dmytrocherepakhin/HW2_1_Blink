#pragma once

#include <cstdint>

#include "Button.h"
#include "Config.h"
#include "Led.h"

enum class OperatingMode : uint8_t
{
    SteadyOn,
    Blinking
};

class App
{
public:
    void init();
    void update();

private:
    void handleButton(uint32_t now);
    void handleLed(uint32_t now);

    Led led_{Config::ledPin};

    Button button_{
        Config::buttonPin,
        Config::debounceDelayMs
    };

    OperatingMode mode_ = OperatingMode::SteadyOn;

    uint32_t lastBlinkTime_ = 0;
};