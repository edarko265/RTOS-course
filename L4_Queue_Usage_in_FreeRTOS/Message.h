#pragma once
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Message carried on the queue
typedef struct {
  uint32_t   id;        // incremental ID
  int32_t    value;     // payload (e.g., sensor/count)
  TickType_t ts_ticks;  // RTOS tick when created
} Message_t;

// Shared queue handle (defined in sketch.ino)
extern QueueHandle_t msg_queue;
