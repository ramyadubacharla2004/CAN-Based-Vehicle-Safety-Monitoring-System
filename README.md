# CAN-Based Vehicle Safety and Monitoring System

## Description

The CAN-Based Vehicle Safety and Monitoring System is an embedded systems project developed using the LPC2129 ARM7 microcontroller and CAN (Controller Area Network) protocol. The system is designed to improve vehicle safety by enabling communication between multiple nodes over the CAN bus.

The project consists of three nodes:

- Main Node
- Indicator Node
- Reverse Node

The Main Node monitors engine temperature using the DS18B20 temperature sensor, receives obstacle information from the Reverse Node, and controls vehicle indicators through the Indicator Node using CAN communication.

The Reverse Node uses an ultrasonic sensor to detect obstacles while reversing and sends warning messages to the Main Node.

The Indicator Node controls the left and right turn indicators based on CAN messages received from the Main Node.

The system provides real-time monitoring and improves vehicle safety by detecting high engine temperature, reverse obstacles, and managing turn indicators efficiently.

## Hardware Used

- LPC2129 ARM7 Microcontroller
- MCP2551 CAN Transceiver
- 16x2 LCD Display
- DS18B20 Temperature Sensor
- HC-SR05 Ultrasonic Sensor
- LEDs
- Push Buttons
- USB to UART Converter

## Software Used

- Embedded C
- Keil uVision
- Flash Magic

## Features

- CAN Bus Communication
- Engine Temperature Monitoring
- Reverse Obstacle Detection
- Left and Right Indicator Control
- LCD Status Display
- Real-Time Vehicle Safety Monitoring

## Advantages

- Reliable real-time communication.
- Reduced wiring using the CAN bus.
- Improved vehicle safety.
- Easy to expand with additional sensors and modules.
- Modular and efficient system design.
  
## Future Scope

- GPS and GSM integration.
- IoT-based vehicle monitoring.
- Mobile application support.
- Tire pressure and fuel level monitoring.
- CAN FD implementation for higher-speed communication.

  ## Conclusion

  This project demonstrates how the CAN protocol enables reliable communication between multiple vehicle modules. It improves vehicle safety through engine temperature monitoring, reverse obstacle detection, and indicator control, while providing a scalable foundation for future automotive applications.
