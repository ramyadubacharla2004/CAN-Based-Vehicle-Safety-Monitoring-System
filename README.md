# 🚗 CAN-BASED VEHICLE SAFETY AND MONITORING SYSTEM

## 📖 Table of Contents

📌 Project Overview

🎯 Objectives

🖼️ Block Diagram

🏗️ System Architecture

⚙️ Hardware Requirements

💻 Software Requirements

📂 Repository Structure

🚀 Features

🖥️ LCD Output Gallery

▶️ Build Instructions

📈 Future Enhancements

👩‍💻 Author

---

## 📌 Project Overview

The **CAN-Based Vehicle Safety and Monitoring System** is an Embedded Systems application developed using the **LPC2129 ARM7 microcontroller** and the **Controller Area Network (CAN)** protocol. The project is designed to improve vehicle safety by monitoring engine temperature, controlling vehicle indicators, and detecting reverse obstacles through communication between multiple CAN nodes.

The system is implemented using **three independent CAN nodes** that communicate with each other over the CAN Bus:

* **Main Node** – Reads engine temperature from the **DS18B20** sensor, displays system information on the **16×2 LCD**, detects Left/Right indicator switch presses, changes vehicle mode (Forward/Reverse), sends indicator commands to the Indicator Node, and receives obstacle information from the Reverse Alert Node.

* **Indicator Node** – Receives CAN messages from the Main Node and controls the Left and Right indicator LEDs.

* **Reverse Alert Node** – Uses the **HC-SR05 Ultrasonic Sensor** to detect obstacles while reversing and sends **SAFE** or **ALERT** messages to the Main Node through CAN communication.

---

## 🎯 Objectives

* Develop a CAN Bus-based vehicle safety system.
* Monitor engine temperature using the DS18B20 sensor.
* Detect reverse obstacles using the HC-SR05 ultrasonic sensor.
* Control Left and Right indicators using CAN communication.
* Enable communication between multiple LPC2129 nodes.
* Learn Embedded C programming and CAN protocol.
* Build a reliable real-time automotive monitoring system.

---

## 🖼️ Block Diagram

<img width="1448" height="1086" alt="image" src="https://github.com/user-attachments/assets/9b56313f-4075-443e-9b60-30edc9740b1f" />


---

## 🏗️ System Architecture

The system is designed using **three LPC2129-based nodes** connected through a CAN Bus.

### 🔹 Main Node

* Reads engine temperature from the DS18B20 sensor.
* Displays temperature and system status on the LCD.
* Detects Left and Right indicator switch presses.
* Switches between Forward and Reverse modes.
* Sends indicator commands to the Indicator Node.
* Receives obstacle status from the Reverse Alert Node.
* Turns ON the warning LED when an obstacle is detected.

### 🔹 Indicator Node

* Receives CAN messages from the Main Node.
* Controls Left and Right indicator LEDs.
* Stops indicators when the stop command is received.

### 🔹 Reverse Alert Node

* Reads distance using the HC-SR05 ultrasonic sensor.
* If the distance is **less than 20 cm**, sends **ALERT**.
* If the distance is **greater than or equal to 20 cm**, sends **SAFE**.
* If no object is detected, the sensor displays **999 cm**.

### 🔹 CAN Bus

* Provides reliable communication between all three nodes using the MCP2551 CAN transceiver.
* Uses **CANH** and **CANL** lines with **120 Ω termination resistors** at both ends for stable communication.

**⚙️ Hardware Requirements**

| Hardware                     | Quantity | Purpose                                       |
| ---------------------------- | -------- | --------------------------------------------- |
| LPC2129 ARM7 Microcontroller | 3        | Main Node, Indicator Node, Reverse Alert Node |
| MCP2551 CAN Transceiver      | 3        | CAN Communication                             |
| DS18B20 Temperature Sensor   | 1        | Engine Temperature Monitoring                 |
| HC-SR05 Ultrasonic Sensor    | 1        | Reverse Obstacle Detection                    |
| 20x4 LCD Display             | 1        | Display Temperature and System Status         |
| LEDs                         | 8        | Left and Right Indicator Control              |
| Warning LED                  | 1        | Obstacle Alert Indication                     |
| Push Buttons                 | 3        | Left, Right and Mode Switch                   |
| USB to UART Converter        | 1        | Program Download and Serial Communication     |
| CAN Bus                      | 1        | Communication Between Nodes                   |
| Power Supply (5V)            | 1        | System Power Supply                           |

**💻 Software Requirements**

| Software     | Purpose                |
| ------------ | ---------------------- |
| Keil µVision | Embedded C Development |
| Embedded C   | Programming Language   |
| Flash Magic  | Programming LPC2129    |
| Proteus      | Circuit Simulation     |

## 📂 Repository Structure

```text
CAN-BASED-VEHICLE-SAFETY-AND-MONITORING-SYSTEM
│   ├── 📁 Images
│   │   ├── Main_Node.jpg
│   │   ├── Indicator_Node.jpg
│   │   ├── Reverse_Node.jpg
│   │   └── ALL_Nodes.jpg
│
├── 📁 Main Node
│   ├── can_major.c
│   ├── can_major.h
│   ├── can_defines_major.h
│   ├── delay_major.c
│   ├── delay_major.h
│   ├── ds18b20_major.c
│   ├── ds18b20_major.h
│   ├── major_lcd.c
│   ├── major_lcd.h
│   ├── interrupt.c
│   ├── main_node.c
│   ├── defines.h
│   ├── major_lcd_defines.h
│   └── major_type.h
│   |__ buzzer_major.c

├── 📁 Indicator Node
│   ├── can_major.c
│   ├── can_major.h
│   ├── can_defines_major.h
│   ├── delay_major.c
│   ├── delay_major.h
│   ├── indicator_node.c
│   └── major_type.h
│
├── 📁 Reverse Alert Node
│   ├── can_major.c
│   ├── can_major.h
│   ├── can_defines_major.h
│   ├── ultrasonic.c
│   ├── delay_major.c
│   ├── delay_major.h
│   ├── reverse_node.c
│   └── major_type.h
│
├── README.md
└── LICENSE
```
## 🚀 Features

- Real-time engine temperature monitoring using the **DS18B20** sensor.
- Reverse obstacle detection using the **HC-SR05** ultrasonic sensor.
- CAN Bus communication between three **LPC2129** nodes.
- Left and Right indicator control through CAN messages.
- Real-time display of engine temperature, vehicle mode, and obstacle status on the **20x4 LCD**.
- Warning LED activation when an obstacle is detected within **20 cm**.
- Reliable and high-speed communication using the **MCP2551 CAN transceiver**.
- Modular design with separate **Main Node**, **Indicator Node**, and **Reverse Alert Node**.
- Developed using **Embedded C** on the **ARM7 LPC2129** microcontroller.

- ## 🖥️ LCD Output Gallery

The system successfully displays real-time vehicle information and safety alerts received through CAN Bus communication.

- Engine Temperature Display
- Forward Mode
- Reverse Mode
- Left Indicator ON
- Right Indicator ON
- SAFE Status
- ALERT Status
- Distance = **999 cm** (No Obstacle Detected)

> <img width="520" height="287" alt="image" src="https://github.com/user-attachments/assets/a93d107c-b2c6-424f-8138-825e8adffd8a" />

>
> ## ▶️ Build Instructions

1. Open the project in **Keil µVision**.
2. Build the project to generate the HEX file.
3. Program the LPC2129 boards using **Flash Magic**.
4. Connect all three nodes using the **CAN Bus (MCP2551)**.
5. Power ON the system.
6. Observe the LCD for engine temperature, vehicle mode, indicator status, and reverse obstacle detection.

7. ## 📈 Future Enhancements

- Add GPS-based vehicle tracking.
- Integrate GSM for emergency alert messages.
- Develop an IoT-based remote vehicle monitoring system.
- Display additional vehicle parameters such as speed and battery status.
- Upgrade to CAN FD for higher data transfer rates.
- Integrate a rear-view camera with obstacle detection.
- Develop a mobile application for live vehicle monitoring.

- ## 👩‍💻 Author

**Ramya Dubacharla**

Embedded system |Embedded C | ARM7 |LPC2129 | CAN 
