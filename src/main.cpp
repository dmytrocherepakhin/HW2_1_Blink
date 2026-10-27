// #include <Arduino.h>

// #define LED 4
// #define BUTTON 5

// u32_t lastLedTimeChange = 0;
// u32_t lastButtonTimeChange = 0;
// u32_t delayTime = 300;
// u32_t debounceDelay = 50;
// bool buttonState = true;
// u8_t lastButtonReading = HIGH;
// u8_t stableButtonReading = HIGH;

// void setup()
// {
//   pinMode(LED, OUTPUT);
//   pinMode(BUTTON, INPUT_PULLUP);
//   Serial.begin(115200);
//   Serial.println("HW2_1 Setuped");
// }

// void loop()
// {
//   u8_t buttonReading = digitalRead(BUTTON);
//   u32_t now = millis();

//   if (buttonReading != lastButtonReading)
//   {
//     lastButtonReading = buttonReading;
//     lastButtonTimeChange = now;
//   }

//   if (now - lastButtonTimeChange >= debounceDelay &&
//       buttonReading != stableButtonReading)
//   {
//     stableButtonReading = buttonReading;

//     if (stableButtonReading == LOW)
//     {
//       buttonState = !buttonState;
//     }
//   }

//   if (buttonState)
//   {
//     digitalWrite(LED, HIGH);
//   }
//   else
//   {
//     if (now - lastLedTimeChange >= delayTime)
//     {
//       lastLedTimeChange = now;
//       digitalWrite(LED, !digitalRead(LED));
//     }
//   }
//   Serial.printf("BUTTON state is: %d    LED state is: %d\n", buttonState, digitalRead(LED));
// }

#include <Arduino.h>

enum class LedState : uint8_t
{
    Off,
    On
};

enum class LedMode : uint8_t
{
    LightOn,
    Blinking
};

struct Config
{
    static constexpr uint8_t ledPin = 4;
    static constexpr uint8_t buttonPin = 5;

    static constexpr uint32_t blinkIntervalMs = 300;
    static constexpr uint32_t debounceDelayMs = 50;

    static constexpr uint32_t serialBaudRate = 115200;
};


class Led
{
public:
    explicit Led(uint8_t pin)
        : pin_(pin)
    {
    }

    void init()
    {
        pinMode(pin_, OUTPUT);
        set(LedState::Off);
    }

    void set(LedState state)
    {
        state_ = state;

        digitalWrite(
            pin_,
            state_ == LedState::On ? HIGH : LOW
        );
    }

    void toggle()
    {
        set(
            state_ == LedState::On
                ? LedState::Off
                : LedState::On
        );
    }

    LedState state() const
    {
        return state_;
    }

private:
    uint8_t pin_;
    LedState state_ = LedState::Off;
};


class Button
{
public:
    explicit Button(uint8_t pin)
        : pin_(pin)
    {
    }

    void init()
    {
        pinMode(pin_, INPUT_PULLUP);

        lastReading_ = digitalRead(pin_);
        stableReading_ = lastReading_;
        lastChangeTime_ = millis();
    }

    bool wasPressed(uint32_t now)
    {
        const uint8_t currentReading = digitalRead(pin_);

        if (currentReading != lastReading_)
        {
            lastReading_ = currentReading;
            lastChangeTime_ = now;
        }

        if ((now - lastChangeTime_ >= Config::debounceDelayMs) &&
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

private:
    uint8_t pin_;

    uint8_t lastReading_ = HIGH;
    uint8_t stableReading_ = HIGH;

    uint32_t lastChangeTime_ = 0;
};

class App
{
public:
    void init()
    {
        Serial.begin(Config::serialBaudRate);

        led_.init();
        button_.init();

        mode_ = LedMode::LightOn;
        led_.set(LedState::On);

        lastBlinkTime_ = millis();

        Serial.println("HW2_1 setup completed");
    }

    void update()
    {
        const uint32_t now = millis();

        handleButton(now);
        handleLed(now);
    }

private:
    void handleButton(uint32_t now)
    {
        if (!button_.wasPressed(now))
        {
            return;
        }

        if (mode_ == LedMode::LightOn)
        {
            mode_ = LedMode::Blinking;
            lastBlinkTime_ = now;

            Serial.println("Mode: Blinking");
        }
        else
        {
            mode_ = LedMode::LightOn;
            led_.set(LedState::On);

            Serial.println("Mode: LightOn");
        }
    }

    void handleLed(uint32_t now)
    {
        if (mode_ != LedMode::Blinking)
        {
            return;
        }

        if (now - lastBlinkTime_ >= Config::blinkIntervalMs)
        {
            lastBlinkTime_ = now;
            led_.toggle();
        }
    }

private:
    Led led_{Config::ledPin};
    Button button_{Config::buttonPin};

    LedMode mode_ = LedMode::LightOn;

    uint32_t lastBlinkTime_ = 0;
};

App& application()
{
    static App instance;
    return instance;
}

void setup()
{
    application().init();
}

void loop()
{
    application().update();
}