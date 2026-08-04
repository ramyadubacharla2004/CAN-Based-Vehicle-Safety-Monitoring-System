🚗 CAN-Based Vehicle Safety and Monitoring System
📖 Table of Contents
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
📌 Project Overview

The CAN-Based Vehicle Safety and Monitoring System is an Embedded Systems project developed using the LPC2129 ARM7 Microcontroller and the Controller Area Network (CAN) protocol. The project enhances vehicle safety by enabling reliable communication between multiple embedded nodes over the CAN bus.

The system consists of three independent CAN nodes:

🔹 Main Node

Monitors engine temperature using the DS18B20 temperature sensor, controls the vehicle indicators based on switch inputs, receives obstacle information from the Reverse Alert Node, and displays the system status on the LCD.

🔹 Indicator Node

Receives CAN messages from the Main Node and controls the left and right vehicle indicators using LEDs.

🔹 Reverse Alert Node

Uses the HC-SR05 Ultrasonic Sensor to measure the distance between the vehicle and nearby obstacles. If an obstacle is detected within 20 cm, it sends an ALERT message to the Main Node; otherwise, it sends a SAFE message.

🎯 Objectives
Develop a CAN Bus-based vehicle safety system.
Monitor engine temperature using the DS18B20 sensor.
Detect reverse obstacles using the HC-SR05 ultrasonic sensor.
Control left and right vehicle indicators using CAN communication.
Display vehicle status and safety alerts on the LCD.
Learn CAN protocol and ARM7 LPC2129 programming.
Build a reliable and real-time automotive embedded system.
🖼️ Block Diagram



🏗️ System Architecture

The project is implemented using three LPC2129-based nodes connected through the CAN Bus.

🔹 Main Node
Reads engine temperature using the DS18B20 sensor.
Displays temperature and vehicle status on the LCD.
Sends left and right indicator commands to the Indicator Node.
Receives obstacle detection status from the Reverse Alert Node.
Activates the warning LED during obstacle detection.
🔹 Indicator Node
Receives CAN messages from the Main Node.
Controls the left and right indicator LEDs based on received commands.
🔹 Reverse Alert Node
Reads obstacle distance using the HC-SR05 ultrasonic sensor.
Sends ALERT when the obstacle is within 20 cm.
Sends SAFE when the obstacle is beyond 20 cm.
🔹 CAN Bus
Provides reliable communication between all three nodes using the MCP2551 CAN Transceiver.
Uses CANH and CANL lines for high-speed and robust communication.
⚙️ Hardware Requirements
Hardware	Quantity	Purpose
LPC2129 ARM7 Microcontroller	3	Main, Indicator, Reverse Alert Nodes
MCP2551 CAN Transceiver	3	CAN Communication
DS18B20 Temperature Sensor	1	Engine Temperature Monitoring
HC-SR05 Ultrasonic Sensor	1	Reverse Obstacle Detection
16×2 LCD Display	1	Display Temperature and Safety Status
LEDs	8	Vehicle Indicator Control
Warning LED	1	Obstacle Alert
Push Buttons	3	Left, Right and Mode Switch
USB to UART Converter	1	Program Download
CAN Bus	1	Communication Between Nodes
Power Supply (5V)	1	System Power
💻 Software Requirements
Software	Purpose
Keil µVision	Code Development
Embedded C	Programming
Flash Magic	Program Download
Proteus	Circuit Simulation
📂 Repository Structure
CAN-Based-Vehicle-Safety-and-Monitoring-System
│
├── 📁 Documentation
│   ├── 📁 Images
│   │   ├── Block_Diagram.png
│   │   ├── Hardware_Setup.jpg
│   │   └── LCD_Output.jpg
│   └── CAN-Based Vehicle Safety and Monitoring System.pdf
│
├── 📁 Main Node
│   ├── can.c
│   ├── can.h
│   ├── delay.c
│   ├── lcd.c
│   ├── ds18b20.c
│   ├── interrupt.c
│   ├── main_node.c
│   └── ...
│
├── 📁 Indicator Node
│   ├── can_major.c
│   ├── can_major.h
│   ├── delay_major.c
│   ├── indicator_node.c
│   └── ...
│
├── 📁 Reverse Alert Node
│   ├── can.c
│   ├── ultrasonic.c
│   ├── reverse_node.c
│   └── ...
│
├── README.md
└── LICENSE
🚀 Features
Real-time engine temperature monitoring.
Reverse obstacle detection using an ultrasonic sensor.
CAN Bus communication between three LPC2129 nodes.
Left and right indicator control through CAN messages.
LCD display for temperature and safety status.
Warning LED activation when an obstacle is detected.
Reliable and high-speed CAN communication.
Modular embedded system design using Embedded C.
🖥️ LCD Output Gallery

The system displays real-time vehicle information and safety alerts.

Engine Temperature Display
Forward Mode
Reverse Mode
SAFE
ALERT
Distance = 999 cm (No Obstacle Detected)




▶️ Build Instructions
Open the project in Keil µVision.
Build the project to generate the HEX file.
Program the LPC2129 boards using Flash Magic.
Connect the Main Node, Indicator Node, and Reverse Alert Node through the CAN Bus.
Power ON all the nodes.
Verify engine temperature monitoring, indicator control, and reverse obstacle detection on the LCD and LEDs.
📈 Future Enhancements
GPS-based vehicle tracking.
GSM emergency alert system.
IoT cloud monitoring.
Mobile application integration.
CAN FD support for higher data rates.
Rear camera integration.
Voice-based warning system.
Engine health monitoring and diagnostics.
👩‍💻 Author

Ramya Dubacharla

Bachelor of Technology (Electronics and Communication Engineering)

Skills: Embedded C | ARM7 LPC2129 | CAN | UART | SPI | I2C | Embedded Systems
