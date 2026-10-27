#include "Button.h"

Button::Button(uint8_t pin, uint32_t debounceDelayMs)
    : pin_(pin),
      debounceDelayMs_(debounceDelayMs)
{
}

void Button::init()
{
    pinMode(pin_, INPUT_PULLUP);

    lastReading_ = digitalRead(pin_);
    stableReading_ = lastReading_;
    lastChangeTime_ = millis();
}

bool Button::wasPressed(uint32_t now)
{
    const uint8_t currentReading = digitalRead(pin_);

    if (currentReading != lastReading_)
    {
        lastReading_ = currentReading;
        lastChangeTime_ = now;
    }

    if ((now - lastChangeTime_ >= debounceDelayMs_) &&
        (currentReading != stableReading_))
    {
        stableReading_ = currentReading;

        if (stableReading_ == LOW)
        {
            return true;
        }
    }

    return false;
}