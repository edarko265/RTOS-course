# 🧠⚙️ FreeRTOS Memory Demo (ESP32) — Stack Overflow → Stable Fix

[![Platform](https://img.shields.io/badge/Platform-ESP32-blue)](#)
[![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-ff6f00)](#)
[![IDE](https://img.shields.io/badge/IDE-Arduino_IDE-00979D)](#)
[![Sim](https://img.shields.io/badge/Simulator-Wokwi-7b2cf5)](#)
[![Status](https://img.shields.io/badge/Status-Production--ready-2ea44f)](#)

> 🚀 A compact, **sleek** demonstration of diagnosing and fixing a **task stack overflow** on ESP32 (FreeRTOS), including instrumentation and a crash-early hook. Comes with simulator + hardware steps and screenshot guidelines.

---

## 📌 TL;DR

- **Issue:** Local `int b[100]` (≈400 B) plus `Serial.println()` on a **too-small task stack** → **stack overflow**.  
- **Fix:** Move buffer **off the stack** (heap/global), **increase stack size** to safe margin, add **high-watermark logs** + **stack-overflow hook**.  
- **Tested on:** Wokwi ESP32 simulator & ESP32 DevKit.

---

## 📚 Background

This task is from:  
🔗 **Source lesson:** *Introduction to RTOS — Memory Allocation / Stack Overflow (ESP32)*  
`04-memory-allocation/esp32-freertos-04-demo-stack-overflow`

---

## 🛠️ What’s Included

- ✅ Fixed, instrumented sketch (drop-in)
- ✅ Stack high-watermark reporting (`uxTaskGetStackHighWaterMark`)
- ✅ FreeRTOS stack overflow hook (`vApplicationStackOverflowHook`)
- ✅ Steps for **Wokwi** and **ESP32 DevKit**
- ✅ Screenshot checklist for submission

---

## 🧩 Fixed Code (Drop-in)

```cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Move large buffer off the task stack
static int *g_buf = nullptr;
static const size_t BUF_LEN = 100;

static void printStackWatermark(const char *tag) {
  UBaseType_t words_remaining = uxTaskGetStackHighWaterMark(NULL);
  Serial.printf("[%s] Stack high-water mark: %u words (~%u bytes)\n",
                tag, (unsigned)words_remaining, (unsigned)(words_remaining * 4));
}

void testTask(void *parameter) {
  int a = 1;
  configASSERT(g_buf != NULL);
  printStackWatermark("testTask start");

  while (1) {
    for (size_t i = 0; i < BUF_LEN; i++) g_buf[i] = a + 1;
    Serial.println(g_buf[0]);

    static uint32_t counter = 0;
    if ((counter++ % 20) == 0) {
      printStackWatermark("testTask loop");
      Serial.printf("[heap] Free heap: %u bytes\n", (unsigned)xPortGetFreeHeapSize());
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

// Crash-early on overflow
extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  ets_printf("\n*** Stack overflow in task: %s ***\n", pcTaskName ? pcTaskName : "unknown");
  abort();
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println();
  Serial.println("--- FreeRTOS Memory Demo (fixed) ---");

  g_buf = (int *)pvPortMalloc(BUF_LEN * sizeof(int));
  if (!g_buf) { Serial.println("FATAL: pvPortMalloc failed"); abort(); }

  const uint32_t STACK_WORDS = 2048; // ~8 KB on ESP32 (words, not bytes!)
  BaseType_t ok = xTaskCreatePinnedToCore(testTask, "Test Task",
                                          STACK_WORDS, NULL, 1, NULL, app_cpu);
  if (ok != pdPASS) { Serial.println("FATAL: xTaskCreatePinnedToCore failed"); abort(); }

  vTaskDelete(NULL); // end Arduino default task
}

void loop() {}
```

---

## 🧪 Expected Output

- Continuous prints of `2` (from `g_buf[0]`)  
- Periodic **stack high-watermark** lines, e.g.:
```
[testTask start] Stack high-water mark: 1600 words (~6400 bytes)
[testTask loop]  Stack high-water mark: 1550 words (~6200 bytes)
[heap] Free heap: 270000 bytes
```
Numbers vary by build, but **keep >300 words** free for safety under typical load.

---

## 🧭 Run on Wokwi (Simulator) 🟣

1. Go to **wokwi.com** → **New Project** → **ESP32 (Arduino)**.
2. Replace the sketch with the **Fixed Code** above.
3. Click **Start** ▶ and open the **Serial Monitor**.
4. Confirm expected output (see above).

### 📸 Screenshots to Capture (for submission)
- Serial Monitor showing:
  - Banner: `--- FreeRTOS Memory Demo (fixed) ---`
  - A few `2` lines, **and**
  - At least one **high-watermark** line
- (Optional) Wokwi wiring diagram view

> Save as: `screenshots/wokwi-serial-output.png`, `screenshots/wokwi-diagram.png`

---

## 🔌 Run on ESP32 DevKit (Hardware) 🔵

1. **Arduino IDE** → Board: `ESP32 Dev Module` (or your specific DevKit).
2. **Tools → Upload Speed**: 921600 (optional), **Partition**: Default.
3. Paste the **Fixed Code** into a sketch, **Upload**.
4. **Serial Monitor** @ **115200 baud**:
   - Look for the banner + high-watermark lines.

### 📸 Screenshots to Capture
- Arduino Serial Monitor with the same expected lines.  
  Save as: `screenshots/hw-serial-output.png`

---

## 🧰 Troubleshooting

- **I still see a stack overflow!**  
  - Ensure `STACK_WORDS` is large enough (remember: **words**, not bytes).  
  - Avoid large local arrays; keep them **global/static** or allocate once on **heap**.

- **Random resets / Guru Meditation**  
  - Check power/USB cable quality.  
  - Remove heavy logging, then re-add gradually.

- **Heap low warnings**  
  - Watch `xPortGetFreeHeapSize()`. Reuse buffers; avoid per-loop `malloc/free`.

---

## ✅ Submission Checklist

- [ ] Wokwi project runs; Serial output matches **Expected Output**  
- [ ] **Screenshot(s)** attached from Wokwi  
- [ ] ESP32 hardware run (optional but recommended)  
- [ ] **Screenshot** attached from Arduino Serial Monitor  
- [ ] README.md included in submission root

---

## 🧠 Why This Fix Works

- **Root cause**: **Task stack** too small for local buffer + `Serial` internals.  
- **Mitigation**: Move large data **off the stack**; provision **adequate stack**; add **runtime introspection** and **fail-fast hook** to surface regressions immediately.

---

## 📎 Project Structure (suggested)

```
.
├── src/
│   └── main.ino               # fixed sketch
├── screenshots/
│   ├── wokwi-serial-output.png
│   ├── wokwi-diagram.png
│   └── hw-serial-output.png
└── README.md                  # this file
```

---

## 🙌 Credits

- Course & reference task: *Introduction to RTOS (ESP32) — Memory Allocation / Stack Overflow*  
- Tooling: **Wokwi** simulator, **Arduino IDE**, **FreeRTOS** (ESP-IDF/Arduino core)

---

### 🧩 Extras (nice to have)

- Use `configCHECK_FOR_STACK_OVERFLOW` in menuconfig (IDF) for deeper checks.  
- Add a **watchdog** to reboot on fatal conditions in production builds.  
- Log high-watermark at **task creation** and after **peak operations** (e.g., prints, complex parsing).

---

> 💡 Pro tip: Once stable, **tune down** `STACK_WORDS` while watching the **high-watermark** to find the minimal safe stack with a comfortable margin.
