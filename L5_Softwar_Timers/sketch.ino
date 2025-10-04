#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

// ESP32-S3 DevKitC-1 pins (Wokwi)
#define LED_PIN 2      // external LED on IO2 via 330Ω resistor
#define ADC_PIN 1      // potentiometer wiper on IO1 (ADC1_CH0)

// Timers
static TimerHandle_t ledTimer = NULL;  // 1s LED blink
static TimerHandle_t adcTimer = NULL;  // 500ms ADC report

// Callbacks
void ledTimerCb(TimerHandle_t) {
  static bool on = false;
  on = !on;
  digitalWrite(LED_PIN, on);
  Serial.printf("[LED] %s @ %lu ms\n", on ? "ON" : "OFF", (unsigned long)millis());
}

void adcTimerCb(TimerHandle_t xTimer) {
  int raw = analogRead(ADC_PIN);          // 0..4095 (12-bit)
  float volts = (3.3f * raw) / 4095.0f;   // assume 3.3V ref
  Serial.printf("[ADC] raw=%4d  V=%.3f  @ %lu ms\n", raw, volts, (unsigned long)millis());

  // For one-shot experiments, if adcTimer was created with pdFALSE,
  // you can manually restart it here:
  // xTimerStart(xTimer, 0);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println("--- ESP32-S3 FreeRTOS Software Timers ---");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  analogReadResolution(12);         // 0..4095
  // analogSetAttenuation(ADC_11db); // optional: extend measurable range

  ledTimer = xTimerCreate("LED", pdMS_TO_TICKS(1000), pdTRUE, (void*)0, ledTimerCb);
  adcTimer = xTimerCreate("ADC", pdMS_TO_TICKS(500),  pdTRUE, (void*)1, adcTimerCb);

  // To compare auto-reload vs one-shot, try:
  // adcTimer = xTimerCreate("ADC", pdMS_TO_TICKS(500), pdFALSE, (void*)1, adcTimerCb);

  if (!ledTimer || !adcTimer) {
    Serial.println("ERROR: timer creation failed");
    while (true) { delay(1000); }
  }

  xTimerStart(ledTimer, 0);
  xTimerStart(adcTimer, 0);

  Serial.println("Timers started: LED every 1000 ms, ADC every 500 ms.");
}

void loop() {
  // Work happens in timer callbacks.
}
