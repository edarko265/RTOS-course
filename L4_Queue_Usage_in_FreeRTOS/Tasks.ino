#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "Message.h"   // <-- same struct, same queue handle

#ifndef LED_PIN
#define LED_PIN 2
#endif

// Receiver: blocks on queue and processes messages
void TaskReceiver(void *param) {
  (void)param;
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Message_t rx{};
  while (true) {
    if (xQueueReceive(msg_queue, &rx, portMAX_DELAY) == pdTRUE) {
      Serial.printf("[Receiver] id=%lu value=%ld ts_ticks=%lu\n",
                    (unsigned long)rx.id,
                    (long)rx.value,
                    (unsigned long)rx.ts_ticks);
      digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // visual cue
    }
  }
}

// Sender: sends a message every 2 seconds
void TaskSender(void *param) {
  (void)param;
  static const uint32_t PERIOD_MS = 2000;

  Message_t tx{};
  TickType_t last_wake = xTaskGetTickCount();

  while (true) {
    tx.id++;
    tx.value    = (int32_t)(tx.id * 10);
    tx.ts_ticks = xTaskGetTickCount();

    if (xQueueSend(msg_queue, &tx, pdMS_TO_TICKS(10)) != pdTRUE) {
      Serial.println("[Sender] Queue full! Dropping message.");
    } else {
      Serial.printf("[Sender] Sent id=%lu value=%ld\n",
                    (unsigned long)tx.id, (long)tx.value);
    }

    vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(PERIOD_MS));
  }
}
