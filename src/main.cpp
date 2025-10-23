//amna iman
//23-ntu-cs-1015

#include <Arduino.h>
#include "esp32-hal-timer.h"

#define LED_PIN 2
#define BUTTON_PIN 4
#define DEBOUNCE_MS 50
#define DEBOUNCE_US (DEBOUNCE_MS * 1000UL)

hw_timer_t* debounceTimer = nullptr;
volatile bool debounceActive = false;

void IRAM_ATTR onDebounceTimer() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  debounceActive = false;
}

void IRAM_ATTR onButtonISR() {
  if (!debounceActive) {
    debounceActive = true;
    // Set the timer alarm for debounce delay
    timerWrite(debounceTimer, 0); // reset counter
    timerAlarmWrite(debounceTimer, DEBOUNCE_US, false);
    timerAlarmEnable(debounceTimer);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(BUTTON_PIN, onButtonISR, FALLING);

  // timerBegin(timer number 0-3, prescaler, countUp)
  debounceTimer = timerBegin(0, 80, true); // 80 prescaler => 1 tick = 1 µs at 80 MHz CPU
  timerAttachInterrupt(debounceTimer, &onDebounceTimer, true);
}

void loop() {
}
