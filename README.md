# 🛌 Eveil — The Anti-Oversleep Ecosystem
 
> **Eveil** is a hardware-software ecosystem designed for chronic oversleepers. Unlike traditional phone alarms that can be snoozed or ignored, Eveil uses **Force Sensitive Resistors (FSRs)** embedded in a bed mat to verify you are physically out of bed before the alarm stops.
 
---
 
## 📖 Table of Contents
 
- [How It Works](#how-it-works)
- [System Architecture](#system-architecture)
- [Components](#components)
- [Wiring](#wiring)
  - [Arduino Pinout](#arduino-pinout)
  - [ESP32 ↔ Arduino UART](#esp32--arduino-uart)
- [Communication Protocol](#communication-protocol)
- [Arduino Command Reference](#arduino-command-reference)
 
---
 
## How It Works
 
1. The **mobile app** connects to the **ESP32** over Bluetooth Low Energy (BLE)
2. At alarm time, the app sends an arm command to the ESP32
3. The ESP32 forwards control commands to the **Arduino** over UART serial
4. The Arduino continuously reads all **4 FSR sensors** and reports pressure data back to the ESP32
5. The alarm only stops when **all FSRs read zero** — meaning the user is out of bed
 
---
 
## System Architecture
 
```
┌─────────────┐        BLE         ┌─────────────┐       UART        ┌─────────────┐
│  Mobile App │ ◄────────────────► │    ESP32    │ ◄───────────────► │   Arduino   │
└─────────────┘                    └─────────────┘                    └──────┬──────┘
                                                                             │
                                                                    ┌────────┴────────┐
                                                                    │                 │
                                                             ┌──────┴──────┐   ┌──────┴──────┐
                                                             │ FSR Sensors │   │Piezo Buzzer │
                                                             │  (×4 mat)   │   │             │
                                                             └─────────────┘   └─────────────┘
```
 
| Controller  | Role |
|-------------|------|
| **ESP32**   | BLE communication, high-level alarm logic, activation countdown, command relay |
| **Arduino** | Real-time FSR reads, buzzer control, pressure reporting |
 
---
 
## Components
 
![Components Overview](https://github.com/user-attachments/assets/a11b97f6-996c-4172-bc29-a0e3ca2c33ff)
 
| Qty | Component | Purpose |
|-----|-----------|---------|
| 1 | ESP32 | BLE + high-level logic |
| 1 | Arduino Uno / Nano | Sensor & buzzer control |
| 4 | Force Sensitive Resistors (FSR) | Pressure detection across bed mat |
| 1 | Piezo Buzzer | Audible alarm output |
| — | Breadboard / PCB | Prototyping |
| — | Jumper Wires | Interconnects |
 
---
 
## Wiring
 
![Wiring Schematic](https://github.com/user-attachments/assets/d336adbf-5e82-45cb-a4b5-caa3c9585ba7)
 
### Arduino Pinout
 
The 4 FSRs are arranged across the mat to detect full-body weight distribution. All four must read zero for the alarm to stop.
 
| Component | Pin | Type |
|-----------|-----|------|
| FSR 1 | `A0` | Analog Input |
| FSR 2 | `A1` | Analog Input |
| FSR 3 | `A2` | Analog Input |
| FSR 4 | `A3` | Analog Input |
| Buzzer | `D8` | Digital Output |
 
### ESP32 ↔ Arduino UART
 
 
| ESP32 Pin | Arduino Pin | Function |
|-----------|-------------|----------|
| `GPIO17` (TX) | `RX` | ESP32 → Arduino |
| `GPIO16` (RX) | `TX` | Arduino → ESP32 |
| `GND` | `GND` | Common Ground ⚠️ |
 
**Baud rate:** `9600` on both sides.
 
---
 
## Communication Protocol
 
The Arduino and ESP32 communicate via newline-terminated strings (`\n`).
 
**Arduino → ESP32** (sensor data, sent continuously while armed):
```
FSR:423,0,817,0
```
Four comma-separated analog readings from A0–A3.
 
**ESP32 → Arduino** (commands):
```
ALARM_START
ALARM_STOP
MANUAL_NOISE
MANUAL_STOP
TEST_ALARM
```
 
---
 
## Arduino Command Reference
 
| Command | Effect |
|---------|--------|
| `ALARM_START` | Arms the system — begins FSR reads and data reporting |
| `ALARM_STOP` | Disarms system, silences buzzer |
| `MANUAL_NOISE` | Forces buzzer on immediately; auto-stops when all FSRs hit zero, re-triggers if pressure returns |
| `MANUAL_STOP` | Kills buzzer and disarms regardless of FSR state |
| `TEST_ALARM` | Runs a 2-second warble test of the buzzer |
