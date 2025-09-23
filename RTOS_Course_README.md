
# ⏱️⚙️ Real‑Time Operating Systems (RTOS) — Course README

[![Course](https://img.shields.io/badge/Course-RTOS%20Fundamentals%20%26%20FreeRTOS-0A84C1)](#)
[![Language](https://img.shields.io/badge/Language-C/C%2B%2B-informational)](#)
[![Platform](https://img.shields.io/badge/Platform-ESP32%20%7C%20STM32%20%7C%20POSIX-blueviolet)](#)
[![License](https://img.shields.io/badge/Use-Education-green)](#)
[![Level](https://img.shields.io/badge/Level-Intermediate-orange)](#)

---

## 🎯 Course Goals
Build the mental model and hands‑on skills to design, program, **predictable** and **safe** embedded systems using an RTOS (with a focus on **FreeRTOS**). You’ll learn to reason about **time**, **resources**, and **faults**—and prove your design works under pressure.

---

## 🗂️ Course Topics Overview
- **Introduction to Real‑Time Systems** — hard vs soft deadlines, latency, jitter, WCET, response time.
- **Real‑Time Scheduling** — rate‑monotonic (RMS), EDF, fixed vs dynamic priorities, feasibility tests.
- **Introduction to FreeRTOS** — tasks, scheduler, tick vs tickless, config options.
- **Resource Management** — mutual exclusion, semaphores, mutexes, priority inheritance, queues.
- **Memory Management** — static vs dynamic, heap schemes, fragmentation, stack sizing.
- **Communications** — queues, stream/buffer APIs, event groups, message passing; ISR ↔ task handoff.
- **Error Handling in RTOS** — watchdogs, asserts, fail‑safe states, brownout reset, logging, recovery.

---

## 👩🏽‍💻 RTOS Programming Topics (Hands‑On)
- **Tasks, Scheduling, Priorities** — task creation, delays, yields, blocking vs busy‑wait.
- **Resource Handling** — binary/counting **semaphores**, **mutex** with priority inheritance, **queues**.
- **Device Interfaces** — **interrupt** handling, ISR constraints, deferred work, DMA interactions.
- **Memory Management** — task stacks, heaps (heap_1…heap_5), static allocation, MPU basics.
- **Error Handling** — deadlock detection/avoidance, **starvation**, **priority inversion** strategies.

---

## 🧪 Learning Outcomes
By the end, you can:
1. Specify timing requirements and argue feasibility using RMS/EDF and response‑time analysis.  
2. Implement multitasking apps in **FreeRTOS** with safe synchronization and communication.  
3. Choose appropriate memory and error‑handling strategies for deterministic behavior.  
4. Integrate ISRs with tasks using queues/semphr and measure end‑to‑end latency.  
5. Diagnose and fix priority inversion, deadlocks, and starvation in real projects.

---

## 🧱 Prerequisites
- Comfortable with **C** (pointers, structs) and basic **microcontroller I/O**.
- Basic digital systems & concurrency concepts (interrupts, timers).
- Toolchain familiarity (Arduino/ESP‑IDF, STM32CubeIDE, or PlatformIO).

---

## 🗓️ Suggested 8‑Week Schedule
| Week | Theme | Labs/Checks |
|---:|---|---|
| 1 | Intro to Real‑Time + Tooling Setup | Build env; GPIO + timer latency probe |
| 2 | Timing Analysis & WCET | Measure WCET; instrument with GPIO toggles |
| 3 | Scheduling (RMS/EDF) | Simulate task sets; feasibility tests |
| 4 | FreeRTOS Essentials | Create tasks; periodic tasks; tick vs tickless |
| 5 | Resources & Comms | Queues, semaphores, mutex PI; producer/consumer |
| 6 | ISRs & Deferred Work | ISR → queue → worker; bounded latency |
| 7 | Memory & Reliability | heap_4 vs static; watchdog; brownout; logging |
| 8 | Faults & Case Study | Inject faults; analyze deadlock, inversion; demo |

---

## 🧱 Reference Architecture (Project Skeleton)
```text
app/
  ├── main.c                # system init, scheduler start
  ├── tasks/
  │   ├── sensor_task.c     # I/O, DMA/ISR handoff
  │   ├── control_task.c    # periodic control loop (RMS)
  │   └── comms_task.c      # queues/stream buffers, UART
  ├── isr/
  │   └── isr_handlers.c    # minimal, defers to tasks
  ├── config/
  │   └── FreeRTOSConfig.h  # tickless, stacks, timers, hooks
  └── platform/
      └── board_io.c        # pins, timers, watchdog
```

---

## 🧭 Assessment (suggested)
- **Lab Exercises** (35%): short weekly labs with latency plots and code review.
- **Midterm Quiz** (15%): concepts (RMS/EDF, WCET, invariants).
- **Capstone Project** (40%): multi‑task app with scheduling proof & fault‑injection report.
- **Participation** (10%): code walkthroughs, design critiques.

---

## 🧰 Tooling
- **FreeRTOS** (vanilla or ESP‑IDF/STM32 HAL port)  
- **PlatformIO / ESP‑IDF / STM32CubeIDE**  
- Logic analyzer or GPIO timing via oscilloscope (optional but ideal)  
- Serial console + RTT logging

---

## 📚 Course Resources (Core & Recommended)
- **Mastering FreeRTOS** (hands‑on text)  
- **Cottet, Delacroix, Mammeri, & Kaiser (2002). *Scheduling in Real‑Time Systems*. Wiley.**  
- **Buttazzo (2011). *Hard Real‑Time Computing Systems* (3rd ed.). Springer.**  
- **Simon (1999). *An Embedded Software Primer*.**  
- **Real‑Time Systems Design and Analysis** (Sha, Laplante, or equivalent modern text)

---

## 🧪 Lab Ideas (Pick & Mix)
- Periodic task framework: drift vs jitter comparison (vTaskDelay vs vTaskDelayUntil).  
- Queue throughput vs latency under contention; back‑pressure demo.  
- Priority inversion scenario with/without **priority inheritance**.  
- Tickless idle power profile; wakeup latency measurement.  
- Deadlock construction & avoidance (lock ordering / try‑locks / timeouts).

---

## ✅ Pass‑Off Checklist
- [ ] Schedulability argument (RMS or EDF) for your capstone.  
- [ ] Worst‑case stack + heap plan with measurement method.  
- [ ] ISR → task handoff bounded and measured.  
- [ ] Recovery plan: watchdog, safe state, error logging.  
- [ ] Demo video + timing plots + README.

---

## 💡 Tips
- Measure everything. “If you didn’t log it, it didn’t happen.”  
- Keep ISRs tiny; defer using queues/semaphores.  
- Prefer **static allocation** for critical tasks; bound all timeouts.  
- Document priorities and the rationale; avoid priority inflation.

---

## 📄 License
Educational use permitted. Cite the sources above when re‑using content.
