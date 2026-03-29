# Eveil
Eveil is a hardware-software application ecosystem that is designed for chronic oversleepers. Instead of purely relying on a phone that can easily be snoozed, this device uses Force Sensitive Resistors.

🔌 Hardware Overview
This project uses an ESP32 and an Arduino (Uno/Nano) working together to create a smart pressure-based alarm system using Force Sensitive Resistors (FSRs) and a buzzer.

The ESP32 handles:

Bluetooth Low Energy (BLE) communication with a mobile app
Command processing (ARM, STOP, manual controls)
Countdown timer before activation

The Arduino handles:
UART recieving serial messages from Esp32
Reading FSR sensor data
Triggering the buzzer
Executing alarm logic in real-time


🧠 System Architecture
[ Mobile App ]
       ↓ (BLE)
    [ ESP32 ]
       ↓ (UART Serial)
    [ Arduino ]
       ↓
 [ FSR Sensors + Buzzer ]


🧩 Components
ESP32 (BLE communication)
Arduino Uno/Nano (sensor + buzzer control)
4 × Force Sensitive Resistors (FSRs)
1 × Piezo Buzzer
Jumper wires
Breadboard / PCB (optional)


<img width="1196" height="862" alt="image" src="https://github.com/user-attachments/assets/a11b97f6-996c-4172-bc29-a0e3ca2c33ff" />



🔗 Wiring
Arduino Connections
| Component | Pin |
| --------- | --- |
| FSR 1     | A0  |
| FSR 2     | A1  |
| FSR 3     | A2  |
| FSR 4     | A3  |
| Buzzer    | D8  |

ESP32 ↔ Arduino (UART)

| ESP32 Pin   | Arduino    |
| ----------- | ---------- |
| GPIO17 (TX) | Arduino RX |
| GPIO16 (RX) | Arduino TX |
| GND         | GND        |



<img width="852" height="577" alt="image" src="https://github.com/user-attachments/assets/d336adbf-5e82-45cb-a4b5-caa3c9585ba7" />
