#include "App.h"

#include <Arduino.h>

void App::init()
{
    Serial.begin(Config::serialBaudRate);

    led_.init();
    button_.init();

    mode_ = OperatingMode::SteadyOn;
    led_.set(LedState::On);

    lastBlinkTime_ = millis();

    Serial.println("HW2_1 setup completed");
}

void App::update()
{
    const uint32_t now = millis();

    handleButton(now);
    handleLed(now);
}

void App::handleButton(uint32_t now)
{
    if (!button_.wasPressed(now))
    {
        return;
    }

    if (mode_ == OperatingMode::SteadyOn)
    {
        mode_ = OperatingMode::Blinking;
        lastBlinkTime_ = now;

        Serial.println("Mode: Blinking");
    }
    else
    {
        mode_ = OperatingMode::SteadyOn;
        led_.set(LedState::On);

        Serial.println("Mode: SteadyOn");
    }
}

void App::handleLed(uint32_t now)
{
    if (mode_ != OperatingMode::Blinking)
    {
        return;
    }

    if (now - lastBlinkTime_ >= Config::blinkIntervalMs)
    {
        lastBlinkTime_ = now;
        led_.toggle();
    }
}