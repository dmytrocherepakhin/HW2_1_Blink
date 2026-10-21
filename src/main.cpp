#include <Arduino.h>

#define LED 4
#define BUTTON 5

u32_t lastLedTimeChange = 0;
u32_t lastButtonTimeChange = 0;
u32_t delayTime = 300;
u32_t debounceDelay = 50;
bool ledIsOn = true;
bool buttonState = true;
u8_t lastButtonReading = HIGH;
u8_t stableButtonReading = HIGH;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("HW2_1 Setuped");
}

void loop()
{
  u8_t buttonReading = digitalRead(BUTTON);
  u32_t now = millis();

  if (buttonReading != lastButtonReading)
  {
    lastButtonReading = buttonReading;
    lastButtonTimeChange = now;
  }

  if (now - lastButtonTimeChange >= debounceDelay &&
      buttonReading != stableButtonReading)
  {
    stableButtonReading = buttonReading;

    if (stableButtonReading == LOW)
    {
      buttonState = !buttonState;
    }
  }

  if (buttonState)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    if (now - lastLedTimeChange >= delayTime)
    {
      lastLedTimeChange = now;
      digitalWrite(LED, ledIsOn ? HIGH : LOW);
      ledIsOn = !ledIsOn;
    }
  }
  Serial.printf("BUTTON state is: %d    LED state is: %d\n", buttonState, ledIsOn);
}
