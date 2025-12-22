# Rove Hardware Connection Diagram

This diagram shows the pin connections for the Rove RC firmware setup with ESP32-S3 N16R8 hardware.

| From          | Pin    | To          | Pin       | Notes                  |
| ------------- | ------ | ----------- | --------- | ---------------------- |
| Battery (+)   | VCC    | ESC Power   | (+)       | 2S/3S LiPo             |
| Battery (-)   | GND    | ESC Power   | (-)       | Common Ground          |
| ESC BEC (5V)  | Output | ESP32-S3    | 5V / VBUS | Powers the board       |
| ESC BEC (GND) | Output | ESP32-S3    | GND       | Shared Ground          |
| ESP32-S3      | GPIO 1 | ESC/Servo 1 | Signal    | Left Motor / Throttle  |
| ESP32-S3      | GPIO 2 | ESC/Servo 2 | Signal    | Right Motor / Steering |

## Connection Details

- **Power Supply**: Connect the battery's positive and negative terminals to the ESC power input
- **ESP32 Power**: Use the ESC's BEC output to power the ESP32-S3 board
- **Motor Control**:
  - GPIO 1 connects to the left motor controller or throttle channel
  - GPIO 2 connects to the right motor controller or steering servo
