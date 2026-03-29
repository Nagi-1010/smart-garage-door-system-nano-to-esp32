# Pinout Tables

## Arduino Nano Version

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

### Arduino Nano Pin Description
- **OPEN_BTN**: Open command push button
- **CLOSE_BTN**: Close command push button
- **STOP_BTN**: Stop command push button
- **LS1_OPEN**: Limit switch for fully open position
- **LS2_CLOSE**: Limit switch for fully closed position
- **MOTOR_IN1 / MOTOR_IN2**: Motor direction control pins
- **EN_PIN**: PWM enable pin for motor speed control
- **LAMP_AJAR**: Door partially open indication lamp
- **LAMP_OPEN**: Fully open indication lamp
- **LAMP_SHUT**: Fully closed indication lamp

---

## ESP32 Version

| Function | Pin |
|---|---|
| OPEN_PIN | GPIO18 |
| STOP_PIN | GPIO19 |
| CLOSE_PIN | GPIO21 |
| OPEN_LS_PIN | GPIO32 |
| CLOSE_LS_PIN | GPIO33 |
| IR_SENSOR_PIN | GPIO23 |

### ESP32 Pin Description
- **OPEN_PIN**: Output signal for opening the door
- **STOP_PIN**: Output signal for stopping the motor
- **CLOSE_PIN**: Output signal for closing the door
- **OPEN_LS_PIN**: Limit switch input for fully open position
- **CLOSE_LS_PIN**: Limit switch input for fully closed position
- **IR_SENSOR_PIN**: Input from IR sensor for automatic opening trigger

---

## Notes
- The Arduino Nano version focused on local control and lamp-based status indication.
- The ESP32 version focused on IoT control and automatic sensing.
- Actual motor driver wiring may depend on the selected L298N module configuration.
- GPIO numbers in ESP32 should be checked carefully before final deployment.
