
# 🟢🟡🔴 ESP32‑S3 Traffic‑Light LEDs (Wokwi + Hardware)

<img width="1470" height="858" alt="Screenshot 2025-09-23 at 13 57 31" src="https://github.com/user-attachments/assets/9ccf463b-9ea2-428e-a02b-db575d78c79a" />


[![Board](https://img.shields.io/badge/board-ESP32--S3--DevKitC--1-0A84C1)](#)
[![Language](https://img.shields.io/badge/lang-Arduino%20(C%2B%2B)-brightgreen)](#)
[![Simulator](https://img.shields.io/badge/sim-Wokwi-purple)](#)
[![License](https://img.shields.io/badge/license-MIT-lightgrey)](#license)

A tiny beginner‑friendly project that blinks three LEDs (green, yellow, red) from an ESP32‑S3 DevKitC‑1—
perfect for learning GPIO output and timing. You can run it entirely in your browser with Wokwi or on real hardware. 
Simulate the exact circuit on Wokwi using the project link in this repo. fileciteturn0file1

---

## ✨ What you’ll learn
- Controlling GPIO pins on ESP32‑S3
- LED wiring (anode → GPIO, cathode → GND)
- Timing and simple state machines (traffic‑light pattern)

---

## 🧰 Bill of Materials
- 1 × **ESP32‑S3 DevKitC‑1**
- 3 × **5 mm LEDs** (Green, Yellow, Red)
- Jumper wires
- *(Optional but recommended)* 3 × **220 Ω resistors** when building on real hardware

> **Note:** Wokwi’s virtual LEDs tolerate direct connection; real LEDs do not. Use series resistors IRL.

---

## 🪛 Wiring / Pinout

| LED | Color  | ESP32‑S3 GPIO | GND |
|-----|--------|---------------|-----|
| D1  | Red    | **GPIO 2**    | Any GND |
| D2  | Yellow | **GPIO 4**    | Any GND |
| D3  | Green  | **GPIO 5**    | Any GND |

The Wokwi diagram connects the anodes of the LEDs to GPIO **2**, **4**, and **5**, and the cathodes to GND. fileciteturn0file0

---

## ▶️ Run in the Browser (Wokwi)

1. Open the Wokwi project (URL included in `wokwi-project.txt`). fileciteturn0file1  
2. Click **Start** ▶️.  
3. Watch the LEDs cycle in the simulator and the boot log appear in the serial monitor.

---

## 💾 Run on Real Hardware

1. Install **Arduino IDE** (or **PlatformIO**) with **ESP32** board support.
2. Open `sketch.ino` and select the board **ESP32S3 Dev Module** (or ESP32‑S3 DevKitC‑1).
3. Plug your board in (USB), select the correct **Port**, then **Upload**.
4. Wire the LEDs exactly as in the table above (use **220 Ω** in series with each LED).

---

## 🧩 Code overview (minimal example)
```cpp
// Pins match the wiring table
const int PIN_RED    = 2;
const int PIN_YELLOW = 4;
const int PIN_GREEN  = 5;

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
}

void loop() {
  // Green
  digitalWrite(PIN_GREEN, HIGH);
  digitalWrite(PIN_YELLOW, LOW);
  digitalWrite(PIN_RED, LOW);
  delay(3000);

  // Yellow
  digitalWrite(PIN_GREEN, LOW);
  digitalWrite(PIN_YELLOW, HIGH);
  digitalWrite(PIN_RED, LOW);
  delay(1000);

  // Red
  digitalWrite(PIN_GREEN, LOW);
  digitalWrite(PIN_YELLOW, LOW);
  digitalWrite(PIN_RED, HIGH);
  delay(3000);
}
```

> Your `sketch.ino` in this repo may include the same or an extended pattern. Use the pin constants above if you adapt it.

---

## 🧪 Troubleshooting
- **Nothing lights up:** Double‑check the board selection and COM/tty port. Ensure 5 V/3V3 isn’t used as a GPIO.
- **LED stuck on/off:** Verify **anode** is on the GPIO pin and **cathode** to **GND**. Reverse if needed.
- **Very dim LEDs on hardware:** You likely forgot the series resistors or used large values—try 220 Ω.
- **Different ESP32 variant:** Update the pin numbers to match free GPIOs on your board.

---

## 📁 Project files
- `diagram.json` – Wokwi hardware layout (ESP32‑S3 + 3 LEDs). fileciteturn0file0
- `wokwi-project.txt` – Link to the online simulator. fileciteturn0file1
- `sketch.ino` – Arduino code for the traffic‑light logic.

---

## 🏷️ License
MIT – do anything you want, just keep the license and copyright notice.

---

## 🙌 Credits
- Wokwi simulation framework
- ESP32‑S3 DevKitC‑1 board by Espressif
- Original circuit authored by the project owner
