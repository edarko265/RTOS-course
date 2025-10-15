#include <Arduino.h>
#include <semphr.h>

// -------------------- Shared state --------------------
static int shared_var = 0;
SemaphoreHandle_t mutex;

// -------------------- Increment task --------------------
void incTask(void *parameters) {
  while (1) {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
      // Critical section: update the shared counter
      shared_var++;
      // Print while still holding the mutex to avoid interleaved lines
      Serial.print("Counter = ");
      Serial.println(shared_var);
      xSemaphoreGive(mutex);
    }
    // Let other tasks run
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

// -------------------- Serial input task --------------------
// Reads an integer from Serial and sets the counter start value using the mutex.
void serialTask(void *parameters) {
  static char buf[32];
  static size_t len = 0;

  Serial.println("Type an integer and press Enter to set the start value.");

  while (1) {
    // Non-blocking read: accumulate until newline
    while (Serial.available() > 0) {
      int c = Serial.read();
      if (c == '\r') continue;          // ignore CR
      if (c == '\n') {                   // got a line
        buf[len] = '\0';
        if (len > 0) {
          // Try to parse integer
          char *endp = nullptr;
          long val = strtol(buf, &endp, 10);
          if (endp != buf && *endp == '\0') {
            // Valid integer -> set shared_var under mutex
            if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
              shared_var = (int)val;
              Serial.print(">> Start value set to ");
              Serial.println(shared_var);
              xSemaphoreGive(mutex);
            }
          } else {
            Serial.print(">> Not an integer: ");
            Serial.println(buf);
          }
        }
        len = 0; // reset buffer
      } else if (len < sizeof(buf) - 1) {
        buf[len++] = (char)c;
      } else {
        // Overflow protection: reset if too long
        len = 0;
        Serial.println(">> Input too long. Try again.");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10)); // yield a bit
  }
}

// -------------------- Setup/Loop --------------------
void setup() {
  Serial.begin(115200);
  while (!Serial) { /* wait for monitor */ }
  Serial.println();
  Serial.println("--- FreeRTOS Mutex Demo (ESP32-S3 / Wokwi) ---");

  mutex = xSemaphoreCreateMutex();
  if (!mutex) {
    Serial.println("Failed to create mutex!");
    while (1) vTaskDelay(pdMS_TO_TICKS(1000));
  }

  // Note: ESP32 FreeRTOS stack size is in words; 2048 words ≈ 8 KB
  xTaskCreate(incTask,   "Increment Task 1", 2048, NULL, 1, NULL);
  xTaskCreate(incTask,   "Increment Task 2", 2048, NULL, 1, NULL);
  xTaskCreate(serialTask,"Serial Task",      3072, NULL, 2, NULL); // a bit larger stack

  // End the Arduino setup task
  vTaskDelete(NULL);
}

void loop() {
  // Not used
}
