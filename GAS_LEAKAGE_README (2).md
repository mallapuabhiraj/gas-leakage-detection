# 🔴 Gas Leakage Detection & Automatic Protection System

<div align="center">

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
┌──────────────────┐   analog (0–1023) ┌──────────────────┐   PWM signal   ┌────────────────┐
│   MQ-135 Gas         │ ──────────────▶│                     │────────────▶│   SG90 Servo       │
│   Sensor             │                   │     Arduino UNO     │                │                   │
│   [ A0 pin ]         │                   │                     │                │  0°  = OPEN       │
└──────────────────┘                   │  if reading > 130    │               │  30° = CLOSED      │
                                           │  → close valve      │               │                    │
                                           │  else → open valve  │               └─────────────────┘
                                           └──────────────────┘
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

---

## 🧠 System Logic

```
  STARTUP
    │
    ├─▶ Warm up MQ-135 (2 min)
    └─▶ Sample 20 readings → set threshold = avg + 50

  LOOP (every 500ms)
    │
    ├─▶ Read gas level from MQ-135
    │
    ├── gasLevel > threshold? ──YES──▶  🔴 Red LED ON
    │                                   🔔 Buzzer ON
    │                                   🔧 Servo → 90° (valve closed)
    │                                   📋 Log "DANGER"
    │
    └── NO ──▶  🟢 Green LED ON
                🔇 Buzzer OFF
                🔧 Servo → 0° (valve open)
                📋 Log "SAFE"
```

---

## 🧰 Bill of Materials

| # | Component | Qty | Cost (INR) |
|---|-----------|:---:|:----------:|
| 1 | Arduino UNO R3 | 1 | ₹249 |
| 2 | MQ-135 Gas Sensor Module | 1 | ₹130 |
| 3 | SG90 Servo Motor | 1 | ₹135 |
| 4 | Active Buzzer | 1 | ₹25 |
| 5 | Red LED + 220Ω Resistor | 1 | ₹5 |
| 6 | Green LED + 220Ω Resistor | 1 | ₹5 |
| 7 | Mini Breadboard | 1 | ₹65 |
| 8 | Jumper Wires (set) | 1 | ₹75 |
| | | **Total** | **~₹689** |

---

## 🔌 Wiring Reference

### Connection Diagram

![Connection Diagram](gas_connection.png)

| Component | Arduino Pin | Notes |
|-----------|:-----------:|-------|
| MQ-135 Analog Out | A0 | Gas concentration (0–1023) |
| MQ-135 VCC | 5V | Sensor heater draws ~150mA |
| MQ-135 GND | GND | — |
| Servo Signal | D9 (PWM) | 0° = open, 90° = closed |
| Buzzer (+) | D7 | Active buzzer, no resistor needed |
| Red LED | D6 | Via 220Ω resistor to GND |
| Green LED | D5 | Via 220Ω resistor to GND |
| All GND | GND | Common ground |

---

## 💻 Source Code

The standout feature is adaptive threshold calibration on startup — no hardcoded values, works in any environment:

```cpp
// Calibrate on startup — no hardcoding needed
int calibrate() {
  Serial.println("Calibrating... keep area clear.");
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += analogRead(GAS_SENSOR_PIN);
    delay(500);
  }
  int baseline = sum / 20;
  return baseline + 50;  // threshold = baseline + safety margin
}

// Alert logic
if (gasLevel > threshold) {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, HIGH);
  gateServo.write(90);          // prototype valve → closed
  Serial.println("DANGER: Gas detected!");
} else {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
  gateServo.write(0);           // prototype valve → open
  Serial.println("SAFE");
}
```

> 📁 Full source: [`src/gas_leakage_detector.ino`](src/gas_leakage_detector.ino)

> 💡 MQ-135 needs ~2 minutes of warm-up before readings stabilise. The calibration loop accounts for this — let it finish before testing.

---

## 📊 Observed Behaviour

| Condition | Sensor Reading | LED | Buzzer | Servo |
|-----------|:--------------:|-----|--------|-------|
| Clean ambient air | ~50–80 | 🟢 Green | OFF | 0° open |
| Mild gas nearby | ~90–120 | 🟢 Green | OFF | 0° open |
| Leakage detected | >130 | 🔴 Red | **ON** | **90° closed** |
| Gas cleared | Returns to baseline | 🟢 Green | OFF | 0° open |

---

## ✅ Strengths

- **Adaptive calibration** — measures its own environment on startup, no manual tuning needed
- **Three simultaneous outputs** — LED + buzzer + servo all fire at once, no single point of notification failure
- **Self-resetting** — automatically recovers when gas clears, no manual reset required
- **Serial logging** — every reading captured, useful for diagnosing when and why alarms triggered

---

## ⚠️ Limitations

- **MQ-135 is non-specific** — detects ammonia, smoke, and CO₂ too; cooking fumes can cause false alarms
- **Servo is prototype-only** — SG90 cannot handle real pipeline pressure; a solenoid valve + relay would be needed for real deployment
- **No persistent log** — history is lost on power cycle; EEPROM or SD card would fix this
- **Single sensor** — one unit has a blind spot; two sensors at different heights improve coverage

---

## 🚀 Roadmap

- [ ] **3-reading confirmation** — require 3 consecutive readings above threshold before triggering, eliminating single-spike false alarms
- [ ] **Solenoid valve + relay** — replace the servo with a real 12V solenoid valve via relay module for actual gas pipe control
- [ ] **GSM alert** — SIM800L module to send an SMS when gas is detected
- [ ] **EEPROM logging** — store last 10 alarm events in non-volatile memory, survives power cycles

---

## 🧠 What I Learned

The first version of this project hardcoded `threshold = 130`. It worked fine on my desk. I moved it to another room and it false-triggered every 30 seconds because the ambient air quality was slightly different there. That failure taught me something important: **a safety system that cries wolf is almost as bad as one that doesn't work** — people start ignoring alarms, which defeats the entire purpose.

The adaptive calibration was the fix, but it required a different way of thinking. Instead of assuming what "safe" looks like, the system measures it. Every deployment gets its own baseline. That's a small change in code but a significant change in how you think about building reliable systems.

The other lesson came from the servo. Watching it snap to 90° the moment gas was detected felt like the project clicking into place — that physical response made the whole thing real in a way that a serial print never does. But it also made the limitation obvious: this is a toy valve. Real safety requires real hardware. Understanding that gap — between what a prototype demonstrates and what production requires — is something I'll carry into every hardware project going forward.

**What I'd do differently:**
- Add the 3-reading confirmation from the start — single spike false alarms are annoying and erode trust in the system
- Log to EEPROM — a safety device with no memory of past events is missing half its value
- Test in multiple environments before calling it done — the hardcoded threshold failure was an avoidable mistake

---

## 🗂️ Repository Structure

```
📦 gas-leakage-detector
 ┣ 📁 src/
 ┃ ┗ 📄 gas_leakage_detector.ino
 ┣ 📁 docs/
 ┃ ┗ 📄 report.pdf
 ┗ 📄 README.md
```

---

## 📄 License

MIT — use it, build on it, stay safe. Drop a ⭐ if it helped.

---

<div align="center">

```
  gas leaks  ──▶  MQ-135 detects  ──▶  Arduino decides  ──▶  system reacts
                                          [ < 500ms ]
              no human required. that's the whole point.
```

</div>
