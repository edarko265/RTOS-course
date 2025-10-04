# 📨 ESP32‑S3 FreeRTOS Queue Demo — TaskSender & TaskReceiver

A minimal, **production‑style** example showing how to pass messages between tasks on an **ESP32‑S3** using **FreeRTOS queues**.  
It runs great on **Wokwi** or physical hardware and demonstrates:
- 🧵 **Two tasks**: `TaskSender` and `TaskReceiver`
- 📦 **Typed messages** (`Message_t`) sent via a **queue**
- ⏱️ **Deterministic periods** with `vTaskDelayUntil`
- 💡 Optional **LED feedback** on each received message

---

## 🔧 What’s Inside

```
├─ Message.h      # Message struct + shared queue declaration
├─ sketch.ino     # Setup/loop, queue creation, task startup
└─ Tasks.ino      # TaskSender + TaskReceiver implementations
```

**Key types**

```cpp
typedef struct {
  uint32_t   id;        // monotonic message ID
  int32_t    value;     // payload (e.g., sensor reading)
  TickType_t ts_ticks;  // RTOS tick captured at creation
} Message_t;
```

---

## 🚀 Quick Start (Wokwi)

1. Create a new project using **ESP32‑S3‑DevKitC‑1**.
2. Add **three tabs** with the exact filenames above and paste the code.
3. Press **Run** and open the **Serial Monitor** (115200 baud).

**Expected output**

```
--- FreeRTOS Queue Demo (ESP32-S3) ---
[Sender] Sent id=1 value=10
[Receiver] id=1 value=10 ts_ticks=12345
[Sender] Sent id=2 value=20
[Receiver] id=2 value=20 ts_ticks=14345
...
```

> 💡 If the queue fills up, you’ll see: `[Sender] Queue full! Dropping message.`

---

## 🧩 Optional Hardware Add‑Ons (for visual/interactive demo)

> The example works with **no external parts**. These are optional:

### 💡 LED (activity indicator)
- **Pin:** `GPIO2` (already used in code)
- **Parts:** 1× LED, 1× 220 Ω resistor  
- **Wiring:** GPIO2 → resistor → LED **anode**; LED **cathode** → GND

### 🔘 Pushbutton (trigger faster sends)
- **Pin:** `GPIO0` → button → **GND**
- **Code tweak:** `pinMode(0, INPUT_PULLUP)` and check `digitalRead(0)` in `TaskSender`

### 🎛️ Potentiometer (send analog values)
- **Pin:** `GPIO1` (ADC)
- **Wiring:** 3.3 V ↔ pot ends ↔ GND, wiper → `GPIO1`
- **Code tweak:** `tx.value = analogRead(1);`

---

## 🛠️ Build Notes & Common Pitfalls

- ✅ **Include order matters** on ESP32‑S3 (Arduino core v3.x): include `Arduino.h` (and FreeRTOS headers) **before** using FreeRTOS types like `BaseType_t`.
- ✅ If you see `BaseType_t does not name a type` → move the `#include <Arduino.h>` **above** your `APP_CPU` declaration.
- ✅ If you see `'Message_t' was not declared in this scope` → ensure `Message.h` is included **before** you call `xQueueCreate(..., sizeof(Message_t))`, and that `Message_t` lives in **Message.h** (not after use).

Example includes at the top of each file:

```cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "Message.h"
```

---

## 🧪 How It Works

- **Queue**: `msg_queue = xQueueCreate(QUEUE_LEN, sizeof(Message_t));`
- **Sender**: builds a `Message_t` every `PERIOD_MS`, then `xQueueSend(...)` (short timeout). Uses `vTaskDelayUntil` for stable cadence.
- **Receiver**: `xQueueReceive(..., portMAX_DELAY)` to **block** until a message arrives, then prints it and toggles the LED.
- **Core pinning**: both tasks pinned to `APP_CPU` for demo repeatability.

---

## 📚 API Cheatsheet

- `xQueueCreate(length, item_size)` — create a queue
- `xQueueSend(queue, &item, ticks_to_wait)` — enqueue
- `xQueueReceive(queue, &item, ticks_to_wait)` — dequeue
- `vTaskDelayUntil(&last_wake, period_ticks)` — periodic loop with low jitter
- `pdMS_TO_TICKS(ms)` — milliseconds → RTOS ticks

---

## 🧩 Extending the Demo

- Swap `value` for a **sensor reading** (ADC, I2C, SPI)
- Add a **second queue** (e.g., command channel back to sender)
- Use **different priorities** to observe scheduling behavior
- Add **timeouts** with `xQueueReceive(..., pdMS_TO_TICKS(100))` and handle “no data” cases

---

## 🧾 License & Attribution

This educational example is inspired by the excellent FreeRTOS tutorials by **Shawn Hymel** and adapted for **ESP32‑S3** with a two‑file structure for clarity.  
Use it freely for learning and projects.

---

## ❤️ Support / Questions

If you get stuck, paste your **error message** and the **top of each file** (includes) into an issue or chat—we’ll sort it out quickly. Happy hacking! 🚀
