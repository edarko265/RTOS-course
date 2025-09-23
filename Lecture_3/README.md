
# ⏱️⚙️ RTOS‑course — ESP32‑S3 FreeRTOS Playground

[![Board](https://img.shields.io/badge/Board-ESP32--S3%20DevKitC--1-0A84C1)](#)
[![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-00B3B3)](#)
[![Language](https://img.shields.io/badge/Language-C/C%2B%2B-informational)](#)
[![Level](https://img.shields.io/badge/Level-Intermediate-orange)](#)
[![License](https://img.shields.io/badge/Use-Education-green)](#)

An educational project that makes **core RTOS concepts** tangible on the **ESP32‑S3**.  
You’ll see **multitasking**, **scheduling**, **resource handling**, and **system diagnostics** in action—
all with clear, well‑commented code.

---

## 📖 Overview

**RTOS‑course** gives you a practical mental model for RTOS design:
- 🌱🔋 **Task Management** — concurrent tasks for LED signalling, serial diagnostics, and CPU workload.
- 🧠📈 **Real‑Time Scheduling** — priorities + fixed‑rate execution using `vTaskDelayUntil()`.
- 💻📊 **System Monitoring** — heap usage + timestamps via serial for quick feedback.
- 🌈🔧 **Practical Hardware** — uses the **on‑board RGB LED** (GPIO 38) of the ESP32‑S3 DevKitC‑1 v1.1.
- ⚙️📦 **Modular Architecture** — clean separation of responsibilities; easy to extend.

---

## 🧪 Demo: Three Concurrent Tasks

The project spawns three FreeRTOS tasks with different priorities:
- **LED** (prio 3) – periodic color changes with deterministic period (400 ms).
- **Serial** (prio 2) – once per second prints free heap and uptime.
- **Compute** (prio 1) – low‑priority CPU load to illustrate preemption.

```cpp
// --- excerpt from main sketch ---
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 38
#define NUM_LEDS 1
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void TaskLED(void*);
void TaskSerial(void*);
void TaskCompute(void*);

void setup() {
  Serial.begin(9600);
  strip.begin(); strip.setBrightness(120); strip.clear(); strip.show();
  xTaskCreate(TaskLED, "LED", 2048, NULL, 3, NULL);
  xTaskCreate(TaskSerial, "SER", 3072, NULL, 2, NULL);
  xTaskCreate(TaskCompute, "CPU", 2048, NULL, 1, NULL);
}
void loop() {} // scheduler runs our tasks
```

> The full example with comments is in this README’s **Example Code** section below—ready to paste into Arduino/PlatformIO.

---

## 🚀 Quick Start

### 1) Clone
```bash
git clone https://github.com/edarko265/RTOS-course
cd RTOS-course
```

### 2) Choose your toolchain
- 🧰 **Arduino IDE**
  1. Install **ESP32** board support and the **Adafruit NeoPixel** library.
  2. Board: **ESP32S3 Dev Module** (or DevKitC‑1 v1.1).
  3. Paste the example into a new sketch and **Upload**.

- ⚙️ **PlatformIO**
  1. Create a new `esp32-s3` project (Arduino framework).
  2. Add dependency: `adafruit/Adafruit NeoPixel`.
  3. Put the example into `src/main.cpp` and **Build → Upload**.

✅ FreeRTOS is included with the ESP32 Arduino core; no separate install needed.

---

## 🧠 Mapping to RTOS Concepts

| Concept | Where to See It |
|---|---|
| **Tasks & Priorities** | `xTaskCreate(..., priority)` for LED/Serial/Compute |
| **Periodic Scheduling** | `vTaskDelayUntil()` for fixed‑rate tasks (LED/Serial/Compute) |
| **Preemption** | Higher‑priority LED task preempts lower‑priority compute |
| **Diagnostics** | `Serial.printf()` + `xPortGetFreeHeapSize()` |
| **Resource Basics** | (Extend) Add queues/semaphores for ISR → task handoff |

---

## 🛠 Example Code (drop‑in)

```cpp
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ===== Onboard RGB LED (ESP32-S3-DevKitC-1 v1.1) =====
#define LED_PIN    38
#define NUM_LEDS   1
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Optional: task handles for diagnostics
TaskHandle_t hLED = nullptr, hSER = nullptr, hCPU = nullptr;

// ---------- Task: LED (highest priority) ----------
void TaskLED(void *pv) {
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(400);   // 400 ms period

  uint32_t colors[] = {
    Adafruit_NeoPixel::Color(255, 0, 0),  // Red
    Adafruit_NeoPixel::Color(0, 255, 0),  // Green
    Adafruit_NeoPixel::Color(0, 0, 255),  // Blue
    0                                      // Off
  };
  int i = 0;

  for (;;) {
    // LED ON for 150 ms
    strip.setPixelColor(0, colors[i]); strip.show();
    vTaskDelay(pdMS_TO_TICKS(150));

    // LED OFF until the next period boundary (fixed-rate schedule)
    strip.setPixelColor(0, 0); strip.show();
    vTaskDelayUntil(&last, period);

    i = (i + 1) % 4; // R -> G -> B -> Off
  }
}

// ---------- Task: Serial (medium priority) ----------
void TaskSerial(void *pv) {
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(1000);  // 1 s
  for (;;) {
    size_t freeHeap = xPortGetFreeHeapSize();
    Serial.printf("[t=%lu ms] freeHeap=%u bytes\n",
                  (unsigned long)millis(),
                  (unsigned int)freeHeap);
    vTaskDelayUntil(&last, period);
  }
}

// ---------- Task: Compute (lowest priority) ----------
void TaskCompute(void *pv) {
  TickType_t last = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(200);   // 200 ms
  volatile uint32_t sink = 0;
  for (;;) {
    // Light CPU load
    for (int k = 0; k < 50000; ++k) {
      sink += (k ^ ((k << 1) + (k >> 1)));
    }
    vTaskDelayUntil(&last, period);
  }
}

void setup() {
  Serial.begin(9600);
  delay(200);
  Serial.println("\nESP32-S3 FreeRTOS demo (LED + Serial + Compute)");

  // Init onboard RGB LED
  strip.begin();
  strip.setBrightness(120);  // 0..255
  strip.clear();
  strip.show();

  // Create tasks (higher number = higher priority)
  xTaskCreate(TaskLED,    "TaskLED",    2048, NULL, 3, &hLED);
  xTaskCreate(TaskSerial, "TaskSerial", 3072, NULL, 2, &hSER);
  xTaskCreate(TaskCompute,"TaskCompute",2048, NULL, 1, &hCPU);
}

void loop() {
  // Not used: FreeRTOS scheduler runs our tasks
}
```

---

## 🔧 Configure & Extend

- **LED Pin**: Using on‑board RGB on **GPIO 38**. Change `LED_PIN` if you use an external LED strip.
- **Priorities**: Tune task priorities to explore preemption and starvation.
- **Tick Rate**: Adjust `configTICK_RATE_HZ` in `FreeRTOSConfig.h` to study jitter and CPU load.
- **Next Steps**: Add a timer ISR that posts to a **queue**; have a worker task consume messages.

---

## 🩺 Troubleshooting

- **Nothing prints** → Check **Port/COM** setting and baud (9600).  
- **LED doesn’t change** → Confirm board is **DevKitC‑1 v1.1** (RGB on GPIO 38) and library installed.  
- **Guru Meditation/Reset** → Increase task stack sizes; remove `delay()` inside tasks; prefer `vTaskDelayUntil()`.

---

## 🎯 Learning Outcomes

- Implement multitasking with FreeRTOS tasks and priorities.  
- Apply periodic scheduling with bounded latency.  
- Use diagnostics (heap/time) to reason about system behavior.  
- Gain confidence building embedded apps on ESP32‑S3.

---

## 📄 License

Educational use permitted. Please credit this project when re‑using in coursework or tutorials.
