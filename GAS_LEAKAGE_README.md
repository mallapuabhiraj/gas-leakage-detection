# 🔴 Gas Leakage Detection & Automatic Protection System

<div align="center">

```
  ╔══════════════════════════════════════════════════════════╗
  ║                                                          ║
  ║   💨 Normal Air          ☠️  Gas Detected                ║
  ║   Sensor: ~50            Sensor: >130                    ║
  ║   Valve:  [ OPEN  ]      Valve:  [ CLOSED ]              ║
  ║   Status: ✅ Safe        Status: 🚨 ALERT                ║
  ║                                                          ║
  ║       » Detects. Decides. Closes the valve. «            ║
  ╚══════════════════════════════════════════════════════════╝
```

![Arduino](https://img.shields.io/badge/Arduino-UNO-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge&logo=cplusplus)
![Domain](https://img.shields.io/badge/Domain-Safety%20Systems-red?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Prototype%20Complete-brightgreen?style=for-the-badge)
![Response](https://img.shields.io/badge/Response-Automatic-orange?style=for-the-badge)

**Skills demonstrated:**
`Embedded C++` · `Analog Gas Sensing` · `Interrupt-Driven Safety Logic` · `Servo Actuation` · `Threshold Calibration` · `Rapid Prototyping`

</div>

---

## 🚨 The Problem

**Gas leaks don't announce themselves.**

LPG, methane, and other harmful gases are colourless and odourless in low concentrations. By the time a person smells something unusual, the concentration may already be at dangerous levels. In homes and industrial kitchens, a slow undetected leak near an ignition source can turn into a disaster in seconds.

> 🇮🇳 India records thousands of LPG-related accidents annually. A large proportion occur not from catastrophic failures, but from slow, undetected leaks that accumulate over time — entirely preventable with early detection.

Traditional approaches rely on human awareness. This project eliminates that dependency entirely.

The question this project set out to answer: *What if the valve just closed itself the moment gas was detected?*

---

## 💼 Business Case

| Pain Point | Real-World Impact |
|-----------|-------------------|
| Gas leaks are invisible until dangerous concentration | No warning → no reaction → accident |
| Human detection is slow and unreliable | People sleep, are distracted, or away |
| Manual valve shutoff requires physical presence | Impossible in a fire or evacuation scenario |

### What this project delivers

An automatic gas leak detector costing under **₹750** that requires zero human intervention can:

- **Detect gas in real time** — MQ-135 continuously monitors air quality, not just on demand
- **React in under 500ms** — servo closes the valve the moment threshold is crossed
- **Reset automatically** — when gas clears, the valve reopens without manual intervention
- **Scale to real deployments** — add a GSM module to SMS alerts, or a relay to cut the main gas supply
- **Provide serial logging** — every gas reading is timestamped for diagnostics

This prototype proves that safety automation doesn't need to be expensive or complex.

---

## 📌 Project Overview

A **fully functional prototype** of an automatic gas leakage detection and valve control system using an Arduino UNO, an MQ-135 gas sensor, and a servo motor acting as a safety valve. The system continuously monitors ambient gas concentration and autonomously rotates the servo to close the valve when gas levels exceed a safe threshold — then reopens it automatically when the air clears.

This project demonstrates: analog gas sensor interfacing, threshold-based safety logic, real-time PWM servo actuation, and serial monitoring — all under ₹750.

---

## ⚙️ System Architecture

```
┌──────────────────┐   analog (0–1023)   ┌─────────────────────┐   PWM signal   ┌──────────────────┐
│   MQ-135 Gas     │ ───────────────────▶│                     │───────────────▶│   SG90 Servo     │
│   Sensor         │                     │     Arduino UNO     │                │                  │
│   [ A0 pin ]     │                     │                     │                │  0°  = OPEN      │
└──────────────────┘                     │  if reading > 130   │                │  30° = CLOSED    │
                                         │  → close valve      │                │                  │
                                         │  else → open valve  │                └──────────────────┘
                                         └─────────────────────┘
                                                   ↑
                                         Sense → Compare → Actuate
                                           (loop every 500ms)
```

**Control Logic:**

1. MQ-135 continuously outputs an analog voltage proportional to gas concentration.
2. Arduino reads this as a value from 0–1023 via `analogRead(A0)`.
3. If the reading **exceeds the threshold (130)** → servo rotates to **30° (valve closed)** + serial alert.
4. If the reading **drops below threshold** → servo returns to **0° (valve open)** + "Safe" logged.
5. Loop repeats every 500ms — fast enough for safety, stable enough to avoid false triggers.

---

## 💻 Source Code

The core safety logic is a simple but effective threshold comparator — read the sensor, compare to a safe limit, actuate accordingly:

```cpp
int gasLevel = analogRead(gasSensorPin);

if (gasLevel > threshold) {
  gateServo.write(30);              // Valve CLOSED — gas detected
  Serial.println("Gas detected! Closing gate.");
} else {
  gateServo.write(0);               // Valve OPEN — air is clear
}
```

> 📁 Full source: [`src/gas_leakage_detector.ino`](src/gas_leakage_detector.ino)

> ⚠️ **Calibration note:** The threshold of `130` was set after observing baseline readings in ambient air (~50–80). Adjust this value for your environment before deployment — always run the sensor for 2–3 minutes first to warm up the heating element.

---

## 🧰 Bill of Materials

| # | Component | Qty | Cost (INR) |
|---|-----------|:---:|------------|
| 1 | Arduino UNO R3 (clone) | 1 | ₹449 |
| 2 | MQ-135 Gas Sensor Module | 1 | ₹130 |
| 3 | SG90 Servo Motor | 1 | ₹135 |
| 4 | Mini Breadboard | 1 | ₹65 |
| 5 | Jumper Wires (set) | 1 | ₹75 |
| | | **Total** | **~₹854** |

> 🛒 All components available at [Robu.in](https://robu.in) or any local electronics market. Prices as of early 2026.

---

## 🔌 Wiring Reference

### Connection Diagram

![Connection Diagram](gas_connection.png)

| Component | Arduino Pin | Wiring Notes |
|-----------|:-----------:|-------------|
| MQ-135 Analog Out | A0 | Main gas concentration signal |
| MQ-135 VCC | 5V | Red wire — sensor needs 5V for heater |
| MQ-135 GND | GND | Black wire |
| Servo Signal | D9 (PWM) | Yellow / orange wire |
| Servo VCC | 5V | Red wire |
| Servo GND | GND | Black wire |

> ⚠️ **Important:** The MQ-135 has a built-in heater that draws ~150mA. Power it from the Arduino's 5V pin only when using a USB supply — for standalone battery use, consider a dedicated 5V regulator.

---

## 📊 Observed Readings

| Condition | Sensor Value (approx.) | Servo Position | Status |
|-----------|----------------------|:--------------:|--------|
| Clean ambient air | ~50 – 80 | 0° | ✅ Safe |
| Near gas source (weak) | ~100 – 120 | 0° | ✅ Safe |
| Gas leakage detected | >130 | 30° | 🚨 Alert |
| After gas clears | Returns to ~50–80 | 0° | ✅ Recovered |

> Threshold set at `130` based on calibration in ambient lab conditions. Your baseline may differ — always calibrate before use.

---

## ✅ Advantages

- **Automatic response** — no human needed; valve closes itself in under 500ms
- **Self-resetting** — system recovers and reopens valve when air clears
- **Under ₹900** — extremely low cost for a functional safety prototype
- **Serial logging** — every reading available for monitoring and diagnostics
- **Upgradeable** — same core logic scales to GSM alerts, IoT dashboards, relay control

---

## ⚠️ Limitations

- **MQ-135 needs warm-up time** — sensor requires ~2 minutes of powered operation before readings stabilise
- **Threshold is environment-specific** — must be recalibrated for each deployment location
- **SG90 is a prototype valve** — not rated for actual gas pipe pressure; a solenoid valve would be needed in production
- **No persistent alerting** — if gas clears before someone notices, there's no record (SD logging would fix this)
- **Single gas type assumption** — MQ-135 detects multiple gases; a false trigger from cooking fumes is possible

---

## 🚀 Roadmap

- [ ] **Buzzer + LED Alert** — add audio-visual alarm alongside the serial output for local notification
- [ ] **GSM Module (SIM800L)** — send an SMS to a phone number the moment gas is detected
- [ ] **Solenoid Valve** — replace the servo with an actual motorised gas valve for real deployment
- [ ] **SD Card Logger** — log every reading with a timestamp to track when and how often leaks occur
- [ ] **OLED Status Display** — show live gas PPM, valve state, and alert history on a small screen

---

## 💡 Ideas to Extend This Project

| Enhancement | Why It Matters | Difficulty |
|-------------|---------------|:----------:|
| 🔔 **Buzzer Alarm** | Loud local alert — makes sure someone nearby always hears it | 🟢 Easy |
| 💡 **Red/Green LED Indicator** | Instant visual status at a glance — no Serial Monitor needed | 🟢 Easy |
| 📟 **OLED Display** | Show live gas level, threshold, and valve state on the device itself | 🟢 Easy |
| 🔄 **Multiple Gas Sensors** | Add MQ-2 (LPG) + MQ-7 (CO) alongside MQ-135 for broader coverage | 🟡 Medium |
| 📱 **SMS Alert via GSM** | SIM800L module texts your phone the moment a leak is detected | 🟡 Medium |
| ☁️ **IoT Dashboard** | ESP8266 + ThingSpeak to log and visualise gas levels remotely | 🟡 Medium |
| 🔧 **Solenoid Valve Control** | Replace servo with a 12V solenoid valve via relay for real gas pipe integration | 🟡 Medium |
| 🧠 **Adaptive Threshold** | Calibrate the safe baseline automatically on startup instead of hardcoding it | 🔴 Advanced |

---

## 🧠 What I Learned

This project seemed simple on paper — read a sensor, move a servo. In practice it taught me considerably more.

**On sensor behaviour**, the MQ-135 warm-up requirement caught me completely off guard. The first few minutes of readings are meaningless noise as the heating element stabilises. Hardcoding a threshold without accounting for this would have caused false alarms on every startup — I only discovered this after watching the serial output and noticing readings settling over time.

**On safety system design**, I learned that a threshold alone isn't enough. The difference between a system that cries wolf and one that reliably alerts is calibration — knowing what "normal" looks like before deciding what "dangerous" means. The threshold of 130 was arrived at empirically, not theoretically.

**On actuation**, watching the servo snap to the closed position the moment the sensor crossed the threshold was one of those satisfying hardware moments. The "read → compare → actuate" loop is the same pattern that underlies industrial SCADA systems, fire suppression controllers, and every safety interlock in manufacturing.

**What I'd do differently:**
- Add a buzzer from the start — a silent serial alert is useless if nobody's watching a laptop
- Use a solenoid valve instead of a servo for anything resembling real deployment — servos aren't rated for pipeline pressure
- Implement a 3-reading confirmation before triggering — one noisy spike shouldn't close a valve; two or three consecutive readings above threshold should

The most valuable takeaway: **safety systems must fail safely.** If the Arduino resets, the servo should default to the closed position — not open. A small but critical design detail I'd prioritise in v2.

---

## 🗂️ Repository Structure

```
📦 gas-leakage-detector
 ┣ 📁 src/
 ┃ ┗ 📄 gas_leakage_detector.ino    ← main firmware
 ┣ 📁 docs/
 ┃ ┗ 📄 report.pdf                  ← project report
 ┗ 📄 README.md
```

---

## 📄 License

Open-source under the [MIT License](LICENSE). Build it, improve it, stay safe — drop a star if it helped. ⭐

---

<div align="center">

```
   💨 ──── sensor reads ──── ⚡ Arduino decides ──── 🔧 valve responds
                    [ all in under 500 milliseconds ]
```

*Built with jumper wires, a ₹130 sensor, and the firm belief that safety shouldn't require a human to be watching.*

</div>
