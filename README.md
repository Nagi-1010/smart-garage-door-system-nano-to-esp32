# Smart Garage Door System: From Arduino Nano to ESP32

A PLC-inspired embedded systems project that evolved from a basic Arduino Nano garage door prototype into a smarter ESP32-based system with IoT control and automatic IR-triggered door opening.

## Overview

This project demonstrates the design and development of a smart garage door control system implemented in two stages.

The first stage was built using **Arduino Nano** as a small-scale **PLC-inspired prototype**. It implemented the main garage door logic using **Open**, **Close**, and **Stop** push buttons, **limit switches**, motor direction control, PWM-based speed control, and three status lamps: **Open**, **Shut**, and **Ajar**.

The second stage upgraded the system to **ESP32**, adding **Wi-Fi connectivity**, **Blynk remote control**, and **IR sensor-based automatic opening**. This version represents the transition from a simple embedded controller to a more intelligent and connected automation system.

## Project Evolution

### Version 1 – Arduino Nano Prototype
- PLC-inspired small-scale garage door control logic
- Open / Close / Stop manual control
- Dual limit switch feedback
- Motor direction and speed control
- Door state indication using lamps
- Designed as a practical embedded implementation of industrial-style garage door behavior

### Version 2 – ESP32 IoT Upgrade
- Replaced Arduino Nano with ESP32
- Added Wi-Fi connectivity
- Added Blynk mobile app control
- Added IR sensor for automatic door opening
- Improved system flexibility and remote interaction
- Demonstrated an upgrade path from local automation to IoT-based control

## Key Features

- PLC-inspired garage door logic
- Motorized open / close / stop control
- Limit switch-based safety and stopping
- Door state indication
- PWM motor speed control in Nano version
- Wireless control using ESP32 and Blynk
- Automatic opening using IR sensor detection
- Multi-network Wi-Fi connection logic
- Practical hardware prototype implementation

## Hardware Components

### Arduino Nano Version
- Arduino Nano
- L298N motor driver
- DC motor / actuator mechanism
- Open push button
- Close push button
- Stop push button
- 2 limit switches
- 3 LEDs for status indication
- Breadboard and jumper wires
- External power source

### ESP32 Version
- ESP32 development board
- L298N motor driver
- DC motor / actuator mechanism
- IR sensor module
- 2 limit switches
- Breadboard and jumper wires
- Wi-Fi connection
- Blynk mobile application
- External power source

## Control Logic

### Arduino Nano Logic
The Arduino Nano version works as a simplified PLC-style control system.  
The user can send **Open**, **Close**, or **Stop** commands using push buttons.  
Two limit switches detect the fully open and fully closed positions.  
The motor direction is controlled through the motor driver, while PWM is used to adjust speed.  
Three lamps indicate the current state of the door:

- **OPEN lamp**: steady ON when the door is fully open, flashing while opening
- **SHUT lamp**: steady ON when the door is fully closed, flashing while closing
- **AJAR lamp**: indicates that the door is neither fully open nor fully closed

### ESP32 Logic
The ESP32 version extends the system by adding remote and automatic functionality.  
The garage door can be controlled through **Blynk** over Wi-Fi.  
An **IR sensor** detects the presence of an object or vehicle and triggers the door to open automatically.  
Limit switches are used to stop the motor when the door reaches its end positions.

## Pin Configuration

### Arduino Nano Version

| Function | Pin |
|---|---|
| OPEN_BTN | D2 |
| CLOSE_BTN | D3 |
| STOP_BTN | D4 |
| LS1_OPEN | D5 |
| LS2_CLOSE | D6 |
| MOTOR_IN1 | D7 |
| MOTOR_IN2 | D8 |
| EN_PIN | D9 |
| LAMP_AJAR | D10 |
| LAMP_OPEN | D11 |
| LAMP_SHUT | D12 |

### ESP32 Version

| Function | Pin |
|---|---|
| OPEN_PIN | GPIO18 |
| STOP_PIN | GPIO19 |
| CLOSE_PIN | GPIO21 |
| OPEN_LS_PIN | GPIO32 |
| CLOSE_LS_PIN | GPIO33 |
| IR_SENSOR_PIN | GPIO23 |

## Project Structure

```text
smart-garage-door-system-nano-to-esp32/
│
├── code/
│   ├── arduino_nano/
│   │   └── garage_door_nano.ino
│   │
│   └── esp32_blynk_ir/
│       ├── garage_door_esp32.ino
│      
├── docs/
│   ├── components_list.md
│   ├── pinout_tables.md
│   └── project_overview.md
│
├── images/
│   ├── logixpro_reference.jpg
│   ├── nano_prototype_1.jpg
│   ├── esp32_prototype_1.jpg
│   ├── ir_sensor_closeup.jpg
│   ├── nano_circuit.jpg
│   └── esp32_circuit.jpg
│
├── LICENSE
└── README.md
