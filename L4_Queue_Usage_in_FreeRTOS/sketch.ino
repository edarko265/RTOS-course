#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "Message.h"   // <-- brings in Message_t

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t APP_CPU = 0;
#else
  static const BaseType_t APP_CPU = 1;
#endif

static const uint8_t  QUEUE_LEN = 8;

// Define the queue handle here (declared extern in Message.h)
QueueHandle_t msg_queue = nullptr;

// Task entry points (implemented in Tasks.ino)
void TaskSender(void *param);
void TaskReceiver(void *param);

void setup() {
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(300));
  Serial.println();
  Serial.println("--- FreeRTOS Queue Demo (ESP32-S3) ---");

  // Create queue to hold Message_t items
  msg_queue = xQueueCreate(QUEUE_LEN, sizeof(Message_t));
  if (msg_queue == nullptr) {
    Serial.println("Failed to create queue. Halting.");
    while (true) { vTaskDelay(pdMS_TO_TICKS(1000)); }
  }

  // Start tasks
  BaseType_t ok1 = xTaskCreatePinnedToCore(
      TaskReceiver, "TaskReceiver", 2048, nullptr, 1, nullptr, APP_CPU);

  BaseType_t ok2 = xTaskCreatePinnedToCore(
      TaskSender, "TaskSender", 2048, nullptr, 1, nullptr, APP_CPU);

  if (ok1 != pdPASS || ok2 != pdPASS) {
    Serial.println("Failed to create one or more tasks. Halting.");
    while (true) { vTaskDelay(pdMS_TO_TICKS(1000)); }
  }
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000)); // not used
}
