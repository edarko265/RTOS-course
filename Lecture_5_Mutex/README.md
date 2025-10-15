# 🔐 FreeRTOS Mutex Demo on ESP32‑S3

An educational project demonstrating how **mutexes** can be used to
**safely share resources** between multiple tasks in a FreeRTOS
environment on the ESP32‑S3.\
This project runs on [Wokwi](https://wokwi.com/) using only the **serial
monitor** --- no additional hardware required. 🚀

------------------------------------------------------------------------

## 🧠 Project Overview

-   Two increment tasks continuously update a shared counter.\
-   A serial input task allows you to set the **start value** of the
    counter at runtime.\
-   A **mutex** ensures that only one task at a time can modify or print
    the shared counter, preventing race conditions.

### ✨ Key Concepts

-   🧭 **Critical Sections** -- Protect shared variables from concurrent
    access.\
-   🧵 **Task Scheduling** -- Multiple tasks run and share a resource.\
-   🪝 **FreeRTOS Mutex** -- Used to synchronize access between tasks.

------------------------------------------------------------------------

## 🧰 Hardware & Software Requirements

-   🪙 **Board**: ESP32‑S3 DevKitC‑1\
-   💻 **Simulator**: [Wokwi](https://wokwi.com/)\
-   🧠 **Framework**: Arduino + FreeRTOS (built‑in on ESP32)

📝 *No extra components are required --- just use the serial monitor.*

------------------------------------------------------------------------

## 🧪 How to Run

1.  Open [Wokwi](https://wokwi.com/) and create a new **ESP32‑S3
    Arduino** project.\
2.  Paste the code from `main.ino` (this repository).\
3.  Press **▶ Run**.\
4.  Open the **Serial Monitor** (115200 baud).\
5.  Type an integer (e.g. `100`) and press `Enter`.\
6.  Watch the counter increment in a thread‑safe way 🧮.

------------------------------------------------------------------------

## 📜 Code Structure

    ├── main.ino            # Main FreeRTOS + Arduino code
    ├── README.md           # Project documentation

### Main Functions

-   `incTask()` → Increments the shared counter.\
-   `serialTask()` → Reads serial input, sets start value.\
-   `xSemaphoreTake()` / `xSemaphoreGive()` → Mutex operations.

------------------------------------------------------------------------

## 🧭 Example Output

    --- FreeRTOS Mutex Demo (ESP32-S3 / Wokwi) ---
    Type an integer and press Enter to set the start value.
    >> Start value set to 100
    Counter = 101
    Counter = 102
    Counter = 103
    ...

🟢 Safe increments, no mixed or overlapping output.

------------------------------------------------------------------------

## 🧑‍💻 Author

**Eric Darko**\
📅 2025\
📡 For educational and demonstration purposes.

------------------------------------------------------------------------

## 🧭 Further Reading

-   [📘 FreeRTOS Mutex
    Documentation](https://www.freertos.org/Embedded-RTOS-Mutexes.html)\
-   [⚡ ESP32 FreeRTOS Reference](https://docs.espressif.com/)\
-   [🧪 Wokwi Simulator](https://wokwi.com/)

------------------------------------------------------------------------

## 🪪 License

This project is released under the **MIT License**.\
Feel free to use and modify for educational purposes.

------------------------------------------------------------------------

✨ *"Good concurrency is invisible. Bad concurrency is a bug."*
