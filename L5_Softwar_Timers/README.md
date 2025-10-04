# ESP32-S3 FreeRTOS Software Timers (Wokwi Demo)  
LED Blink + ADC Streaming

![ESP32](https://img.shields.io/badge/ESP32--S3-DevKitC--1-000?logo=espressif&logoColor=white) ![Arduino](https://img.shields.io/badge/Arduino-IDE%20%2F%20Core-00979D?logo=arduino&logoColor=white) ![FreeRTOS](https://img.shields.io/badge/FreeRTOS-Software%20Timers-4353FF) ![Wokwi](https://img.shields.io/badge/Wokwi-Simulator-5F43FF)  

---

## ✨ What this project does
- ⏱️ Demonstrates **FreeRTOS Software Timers** on the ESP32-S3  
- 💡 **Timer A** (auto-reload, 1000 ms): blinks an external LED on **IO2**  
- 📈 **Timer B** (auto-reload, 500 ms): reads ADC from a potentiometer on **IO1** and prints to **Serial**  
- 🔁 Includes an easy switch to test **one-shot** timers vs **auto-reload**

---

## 📦 Bill of Materials
- ESP32-S3 DevKitC-1 (Wokwi board id: `board-esp32-s3-devkitc-1`)  
- 1× LED (any color)  
- 1× **330 Ω** resistor (series with LED)  
- 1× Potentiometer (10 kΩ recommended)  
- Wokwi online simulator (no physical hardware required)

---

## 🗺️ Pin Map & Wiring

### Potentiometer
| Pot Pin | ESP32-S3 Pin | Notes |
|---|---|---|
| VCC | **3V3.1** | 3.3 V |
| GND | **GND.4** | Ground |
| SIG (wiper) | **IO1** | ADC1_CH0 |

### LED + Resistor
| From | To | Notes |
|---|---|---|
| **IO2** | Resistor leg 1 | GPIO output |
| Resistor leg 2 | **LED Anode (A)** | Long leg |
| **LED Cathode (C)** | **GND.3** | Short leg / flat side to GND |

> 🔎 We use an **external LED** because the S3 DevKitC-1 doesn’t guarantee an onboard LED in Wokwi.

---

## 🧩 Project Files

### `diagram.json` (drop-in)
```json
{
  "version": 1,
  "author": "Eric Darko",
  "editor": "wokwi",
  "parts": [
    { "type": "board-esp32-s3-devkitc-1", "id": "esp", "top": 0, "left": 0, "attrs": {} },
    { "type": "wokwi-potentiometer", "id": "pot1", "top": -10, "left": 240, "attrs": { "value": "50" } },
    { "type": "wokwi-resistor", "id": "r1", "top": 100.8, "left": 172.25, "rotate": 90, "attrs": { "value": "330" } },
    { "type": "wokwi-led", "id": "led1", "top": 130.8, "left": 157.4, "attrs": { "color": "red" } }
  ],
  "connections": [
    [ "pot1:GND", "esp:GND.4", "black", [ "v171.9", "h-173" ] ],
    [ "pot1:VCC", "esp:3V3.1", "red", [ "v0" ] ],
    [ "pot1:SIG", "esp:1", "green", [ "v0" ] ],
    [ "esp:2", "r1:1", "green", [ "h0" ] ],
    [ "r1:2", "led1:A", "green", [ "h0" ] ],
    [ "led1:C", "esp:GND.3", "green", [ "v0" ] ],

    // Serial monitor shorthand (UART0)
    [ "esp:TX", "$serialMonitor:RX", "" ],
    [ "esp:RX", "$serialMonitor:TX", "" ]
  ],
  "dependencies": {}
}
```

### `sketch.ino`
```cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

// ESP32-S3 DevKitC-1 pins (Wokwi)
#define LED_PIN 2      // external LED on IO2 via 330Ω resistor
#define ADC_PIN 1      // potentiometer wiper on IO1 (ADC1_CH0)

// Timers
static TimerHandle_t ledTimer = NULL;  // 1s LED blink (auto-reload)
static TimerHandle_t adcTimer = NULL;  // 500ms ADC report (auto-reload)

// Callbacks
void ledTimerCb(TimerHandle_t) {
  static bool on = false;
  on = !on;
  digitalWrite(LED_PIN, on);
  Serial.printf("[LED] %s @ %lu ms\n", on ? "ON" : "OFF", (unsigned long)millis());
}

void adcTimerCb(TimerHandle_t xTimer) {
  int raw = analogRead(ADC_PIN);          // 0..4095 (12-bit)
  float volts = (3.3f * raw) / 4095.0f;   // 3.3V reference
  Serial.printf("[ADC] raw=%4d  V=%.3f  @ %lu ms\n", raw, volts, (unsigned long)millis());

  // If created as one-shot (pdFALSE), uncomment to restart manually:
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

  // Auto-reload timers
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
```

---

## ▶️ Quick Start (Wokwi)
1. Go to **wokwi.com** → **New Project** → **ESP32-S3 DevKitC-1**.  
2. Replace `diagram.json` and `sketch.ino` with the files above.  
3. Click **Run** ▶️.  
4. The **Serial Monitor** auto-opens (wired via `$serialMonitor`); set **115200** baud if needed.  
5. Turn the potentiometer and watch ADC prints every **500 ms**.  
6. See the LED on **IO2** blink every **1 s**.

---

## 🧠 FreeRTOS Timers: Auto-Reload vs One-Shot

| Mode | Create Flag | Behavior | Typical Use |
|---|---|---|---|
| Auto-reload | `pdTRUE` | Fires periodically at fixed interval | Heartbeats, periodic sampling |
| One-shot | `pdFALSE` | Fires **once** then stops | Timeouts, delayed actions |

**Try it now:**  
Change `adcTimer` to one-shot in `setup()`:
```cpp
adcTimer = xTimerCreate("ADC", pdMS_TO_TICKS(500), pdFALSE, (void*)1, adcTimerCb);
```
Then **manually restart** it at the end of `adcTimerCb()`:
```cpp
xTimerStart(xTimer, 0);
```

---

## 🔍 Expected Serial Output
```
--- ESP32-S3 FreeRTOS Software Timers ---
Timers started: LED every 1000 ms, ADC every 500 ms.
[LED] ON @ 1002 ms
[ADC] raw= 512  V=0.413  @ 1502 ms
[LED] OFF @ 2002 ms
[ADC] raw= 845  V=0.681  @ 2003 ms
...
```

---

## 🧩 Notes & Tips
- **ADC**: Prefer **ADC1** pins on ESP32-S3 for analog reads. We use **IO1 (ADC1_CH0)**.  
- **Voltage**: Don’t exceed **3.3 V** on analog inputs.  
- **LED current**: Keep the **330 Ω** series resistor to protect the GPIO.  
- **Serial panel missing?** Use Wokwi `$serialMonitor` shorthand (already included) or add a `wokwi-serial-monitor` part.

---

## 🛠️ Troubleshooting
- **No Serial output** → Ensure `Serial.begin(115200);`, check baud, hit **Reset** (board icon)  
- **ADC stuck** → Confirm pot SIG is on **IO1**, and VCC/GND wired correctly  
- **LED not blinking** → Check LED polarity (long leg = **A**), verify **IO2** in both wiring and code  
- **Timers not firing** → Confirm `xTimerCreate` returned non-`NULL` and both `xTimerStart(...)` calls succeed

---

## 📜 License
MIT (or your preferred license)

---

## 🙌 Credits
- Built with **ESP32-S3 DevKitC-1**, **Arduino Core**, **FreeRTOS**, and **Wokwi**.  
- Project author: **Eric Darko**.
