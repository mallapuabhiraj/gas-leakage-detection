# ⚠️ Gas Leakage Detection & Automatic Protection System

<div align="center">

```
  ██████████████████████████████████████████████████
  █                                                █
  █   SYSTEM STATUS          GAS LEVEL   [ 052 ]  █
  █   ─────────────          ─────────────────     █
  █   🟢 SAFE                VALVE  : [ OPEN  ]   █
  █   🔴 DANGER  ←──────     BUZZER : [ OFF   ]   █
  █                          LED    : [GREEN  ]   █
  █   threshold: 130                              █
  ██████████████████████████████████████████████████
        detect → decide → close valve → alert
```

![Arduino](https://img.shields.io/badge/Arduino-UNO-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Sensor](https://img.shields.io/badge/Sensor-MQ--135-red?style=for-the-badge)
![Safety](https://img.shields.io/badge/Type-Safety%20Critical-critical?style=for-the-badge)
![Response](https://img.shields.io/badge/Response%20Time-%3C500ms-brightgreen?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)

**Core skills:** `Analog Gas Sensing` · `Adaptive Threshold Calibration` · `Interrupt Safety Logic` · `Multi-Output Alerting` · `Fail-Safe Design`

</div>

---

## ☠️ Why This Exists

Every year in India, hundreds of people are killed or injured by LPG and gas leaks — not from explosions, but from slow, invisible accumulation. Colourless. Odourless at low concentrations. Completely undetectable by humans until it's already dangerous.

The worst part? Most incidents happen while people are asleep, distracted, or simply not in the room. By the time someone notices the smell, the concentration may already be at explosive levels.

**This project removes the human from the equation entirely.**

No one needs to smell anything. No one needs to be home. The moment gas is detected — the valve closes, the buzzer screams, the red LED flashes, and the serial log records it. Automatically. In under 500 milliseconds.

---

## 🧠 How The System Thinks

```
STARTUP
  │
  ├─▶ Warm-up sensor (2 min)
  ├─▶ Sample 20 baseline readings
  ├─▶ Calculate adaptive threshold = baseline_avg + 50
  │
MONITORING LOOP (every 500ms)
  │
  ├─▶ Read MQ-135 → gasLevel
  │
  ├─▶ gasLevel > threshold?
  │       │
  │    YES│                           NO
  │       ▼                           ▼
  │   🔴 RED LED ON             🟢 GREEN LED ON
  │   🔔 BUZZER ON              🔇 BUZZER OFF
  │   🔧 SERVO → 90°            🔧 SERVO → 0°
  │   ⚡ RELAY → closes         ⚡ RELAY → opens
  │      solenoid valve            solenoid valve
  │   📋 Log "DANGER"           📋 Log "SAFE"
  │       │                           │
  └───────┴───────────────────────────┘
                    LOOP
```

---

## 📌 What This Project Does

A **fully wired safety prototype** using an Arduino UNO, MQ-135 gas sensor, servo motor (simulating a valve), 5V relay (driving a solenoid valve), red/green LEDs, and a buzzer. The system:

- **Calibrates itself on startup** — reads 20 baseline samples and sets the threshold dynamically so it works in any environment without hardcoding
- **Closes the gas valve** via servo + relay the moment gas exceeds the threshold
- **Fires the buzzer** in a repeating alert pattern — not a single beep, a sustained alarm
- **Shows status via LEDs** — green = safe, red = danger, no ambiguity
- **Logs every event** to Serial — timestamped readings for diagnostics
- **Resets automatically** — when gas clears, valve reopens, buzzer stops, green LED resumes

---

## 🧰 Bill of Materials

| # | Component | Qty | Cost (INR) |
|---|-----------|:---:|:----------:|
| 1 | Arduino UNO R3 | 1 | ₹449 |
| 2 | MQ-135 Gas Sensor Module | 1 | ₹130 |
| 3 | SG90 Servo Motor *(prototype valve)* | 1 | ₹135 |
| 4 | 5V Single Channel Relay Module | 1 | ₹99 |
| 5 | 12V Solenoid Valve (Normally Closed) | 1 | ₹193 |
| 6 | Active Buzzer | 1 | ₹25 |
| 7 | Red LED + 220Ω Resistor | 1 | ₹5 |
| 8 | Green LED + 220Ω Resistor | 1 | ₹5 |
| 9 | Mini Breadboard | 1 | ₹65 |
| 10 | Jumper Wires (set) | 1 | ₹75 |
| | | **Total** | **~₹1,181** |

> 🛒 Prices sourced from [Robu.in](https://robu.in) and [Robocraze](https://robocraze.com), early 2026.

---

## 🔌 Wiring Reference

### Connection Diagram

![Connection Diagram](gas_connection.png)

| Component | Arduino Pin | Notes |
|-----------|:-----------:|-------|
| MQ-135 Analog Out | A0 | Gas concentration signal (0–1023) |
| MQ-135 VCC | 5V | Sensor heater draws ~150mA |
| MQ-135 GND | GND | — |
| Servo Signal | D9 (PWM) | 0° = open, 90° = closed |
| Relay Signal | D8 | Controls solenoid valve |
| Buzzer (+) | D7 | Active buzzer — no resistor needed |
| Red LED | D6 | Via 220Ω resistor to GND |
| Green LED | D5 | Via 220Ω resistor to GND |
| All VCC | 5V | — |
| All GND | GND | Common ground |

> ⚠️ The solenoid valve runs on **12V** — do NOT power it directly from Arduino 5V. Use the relay module to switch a separate 12V supply to the valve. The relay's control side runs from D8 at 5V.

---

## 💻 Source Code

The key upgrade over basic implementations is **adaptive threshold calibration** on startup — no hardcoded values, no recalibration needed when you move the device:

```cpp
// --- Adaptive Threshold: calibrate on startup ---
int calibrate() {
  Serial.println("Calibrating... keep area clear of gas.");
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += analogRead(GAS_SENSOR_PIN);
    delay(500);
  }
  int baseline = sum / 20;
  Serial.print("Baseline: "); Serial.println(baseline);
  return baseline + 50;   // threshold = baseline + safety margin
}

// --- Core alert logic ---
if (gasLevel > threshold) {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, HIGH);
  gateServo.write(90);           // valve closed
  digitalWrite(RELAY_PIN, HIGH); // solenoid closed
} else {
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
  gateServo.write(0);            // valve open
  digitalWrite(RELAY_PIN, LOW);  // solenoid open
}
```

> 📁 Full source: [`src/gas_leakage_detector.ino`](src/gas_leakage_detector.ino)

> 💡 The MQ-135 needs ~2 minutes of warm-up before readings stabilise. The calibration routine accounts for this — don't skip it.

---

## 📊 Observed Behaviour

| Condition | Sensor Reading | LEDs | Buzzer | Valve | Relay |
|-----------|:--------------:|------|--------|-------|-------|
| Clean air (calibrated) | ~50–80 | 🟢 Green | OFF | OPEN | OFF |
| Mild gas presence | ~90–120 | 🟢 Green | OFF | OPEN | OFF |
| Leakage detected | >130 | 🔴 Red | **ON** | **CLOSED** | **ON** |
| Gas cleared | Returns to baseline | 🟢 Green | OFF | OPEN | OFF |

---

## ✅ What Works Well

- **Adaptive calibration** means it works anywhere — bedroom, kitchen, lab — without touching the code
- **Multi-output alerting** — LEDs + buzzer + valve + relay all fire simultaneously, no single point of failure for notification
- **Self-resetting** — system recovers and reopens valve automatically when air clears
- **Serial diagnostic log** — every reading captured for post-incident review
- **Fail-safe relay logic** — solenoid valve is Normally Closed, so a power loss = valve stays shut by default

---

## ⚠️ Known Limitations

- **MQ-135 is non-specific** — it detects ammonia, benzene, CO₂, and smoke too; cooking fumes can false-trigger
- **Servo is prototype-only** — SG90 cannot handle real pipeline pressure; solenoid valve needed for real use
- **No persistent logging** — power cycle wipes the history; SD card or EEPROM would fix this
- **Single sensor** — one sensor has a blind spot; two sensors at different heights would improve coverage
- **No SMS/app alert** — local alarm only; add SIM800L for remote notification

---

## 🚀 What's Next

- [ ] **3-reading confirmation** — trigger alarm only after 3 consecutive readings above threshold, eliminating single-spike false alarms
- [ ] **Buzzer pattern escalation** — slow beep at warning level, fast continuous at danger level
- [ ] **EEPROM event log** — write last 10 alarm events to non-volatile memory, survives power cycles
- [ ] **GSM/SMS alert** — SIM800L module to text a phone number on detection
- [ ] **Second MQ-2 sensor** — specifically tuned for LPG/methane, combined with MQ-135 for dual-confirmation

---

## 🧠 What I Learned

Honestly the most important thing I learned from this project has nothing to do with code — it's about **what a safety system actually means**.

My first version hardcoded `threshold = 130`. It worked in the lab. I tested it, it detected gas, done. But then I moved it to a different room and it false-triggered constantly because the ambient air quality was slightly different. That's when I understood that a safety system which cries wolf is almost as dangerous as one that doesn't work — people stop trusting it and start ignoring alarms.

The adaptive calibration was the solution but it required a mindset shift: **don't assume what "normal" looks like — measure it.** Let the device figure out its own environment before deciding what dangerous means.

The second big lesson was **fail-safe design**. Early on my relay logic had the solenoid valve default to OPEN when unpowered. Someone pointed out that if the Arduino crashes mid-leak, the valve stays open. Flipping to a Normally Closed solenoid — where you need active power to keep it open — means any failure mode results in the valve closing. The safe state should be the default state, not the powered state.

**What I'd build differently:**
- Add EEPROM logging from day one — you want a record of when alarms fired even if nobody was watching
- Use two sensors at different heights — gas distribution isn't uniform and a single sensor has a real blind spot
- Never trust a single reading — the 3-sample confirmation before triggering would have saved several false alarms during testing

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

MIT License — use it, build on it, stay safe. If this helps you, drop a ⭐

---

<div align="center">

```
  💨 gas leaks           ⚡ arduino reacts          🔒 valve closes
  [ invisible ]    ───▶  [ in 500ms ]         ───▶  [ automatically ]

           no human required. that's the whole point.
```

</div>
