# Rove RC Firmware

**Rove** is a high-performance, non-blocking RC firmware designed to retrofit toy-grade RC cars with the precision of an **Xbox Wireless Controller**. By leveraging the dual-core power and 8MB of PSRAM on the **ESP32-S3**, Rove delivers a lag-free driving experience that replaces cheap, unresponsive stock transmitters.

## Key Features

Rove transforms your RC car with these powerful capabilities:

- **Dual-Mode Operation**: Seamlessly switch between **Tank Mode** (differential steering for tracked or twin-motor vehicles) and **Car Mode** (standard motor with a steering servo) using the **Y Button**
- **Intuitive Control**: Arcade-style driving using the **Left Stick** - Y-axis controls speed, X-axis handles direction
- **Smart Safety**: Automatic failsafe that sets all motor outputs to neutral when Bluetooth connection to your Xbox controller is lost
- **Software Watchdog Timer**: 2-second timeout protection against system hangs or crashes
- **High Performance**: Leverages the ESP32-S3's dual-core processor and 8MB PSRAM for smooth, responsive control

## Controller Mappings

The firmware uses an **Xbox Wireless Controller** (or any XInput-compatible gamepad). Here are the controls for each driving mode:

### Car Mode (Standard RC)

_For vehicles with a steering servo and single motor/ESC_

| Input                     | Function    | Description                              |
| ------------------------- | ----------- | ---------------------------------------- |
| **Left Stick Up/Down**    | Throttle    | Forward/Reverse control (Y-axis)         |
| **Left Stick Left/Right** | Steering    | Turn left/right (X-axis)                 |
| **Y Button**              | Mode Switch | Switch to Tank Mode                      |
| **B Button**              | Brake/Stop  | Apply brake, stop all motors immediately |

### Tank Mode (Differential Steering)

_For tracked vehicles or twin-motor cars_

| Input                     | Function        | Description                                                    |
| ------------------------- | --------------- | -------------------------------------------------------------- |
| **Left Stick Up/Down**    | Forward/Reverse | Both motors move forward or reverse together                   |
| **Left Stick Left/Right** | Turn            | Turns the vehicle by differentiating motor speeds (left/right) |
| **Y Button**              | Mode Switch     | Switch to Car Mode                                             |
| **B Button**              | Brake/Stop      | Apply brake, stop all motors immediately                       |

### Xbox Controller Layout Reference

```
          ┌─────────────┐
    ┌─────│ LB   RB     │─────┐
         │  ┌───┐       │
         │  │◄┼▼►│  A    │
         │  ├──┤       │
    ◄────│► │◄┼▲►│  B    │────►
         │  │   │       │
         │  └───┘       │
    ◄────│► │ Start │ Select│────►
         │  │  Y      │     │
         │  └─────────┘     │
    ┌─────│                   │─────┐
          └───────────────────┘

Left Stick: Controls movement (forward/back, left/right)
Y Button: Top face button - switches driving modes
B Button: Bottom face button - brake/stop function
```

## Safety Features

### Failsafe Behavior

When the controller disconnects or turns off while accelerating:

1. **Immediate Detection**: Bluepad32 detects connection loss within milliseconds
2. **Motor Neutralization**: All motors immediately set to neutral (1500µs PWM)
3. **Safe State Persists**: System remains in failsafe until controller reconnects
4. **Serial Logging**: Warning message logged via USB CDC for debugging

### Software Watchdog Timer

- Monitors system activity with 2-second timeout
- If no activity detected, automatically sets motors to safe position
- Prevents runaway conditions from software hangs or infinite loops

## Hardware Overview

The ESP32-S3 serves as the central hub for your RC car, keeping the interior clean and organized. Most retrofits will use the **BEC (Battery Eliminator Circuit)** from your ESC to power the board directly.

### Wiring Connections

```
                    ┌─────────────────────┐
                    │   ESP32-S3 N16R8    │
                    │                     │
  Battery (+) ◄─────│ VCC / VBUS          │
  Battery (-) ◄─────│ GND                 │
                    │                     │
  ESC BEC 5V ───────│ 5V                  │
  ESC BEC GND ──────│ GND                 │
                    │                     │
  Left Motor/ESC ───│ GPIO 1              │
  Right Motor/Servo├─│ GPIO 2              │
                    │                     │
                    └─────────────────────┘

Note: Use ESP32Servo library for PWM output (GPIO 1 & 2)
```

| Component         | ESP32-S3 Pin | Function                            |
| ----------------- | ------------ | ----------------------------------- |
| **ESC BEC (5V)**  | 5V / VBUS    | Powers the MCU                      |
| **ESC BEC (GND)** | GND          | Shared Ground                       |
| **ESC/Servo 1**   | GPIO 1       | Left Motor (Tank) / Throttle (Car)  |
| **ESC/Servo 2**   | GPIO 2       | Right Motor (Tank) / Steering (Car) |

### Pin Layout Diagram

```
ESP32-S3 DEVKITC-1 N16R8 Pinout:

    ┌──────────────────────────────┐
    │          ESP32-S3            │
    │                              │
   5V│ ○─── VCC / VBUS            │  Battery Power Input
 GND│ ○─── GND                    │  Common Ground
   1│ ○─── GPIO 1                 │  Left Motor PWM Output
   2│ ○─── GPIO 2                 │  Right Motor/Servo PWM Output
    │                              │
    │     PSRAM (8MB)              │
    │     Flash (16MB)             │
    └──────────────────────────────┘
```

## PlatformIO Configuration

For optimal performance with your **ESP32-S3 N16R8** hardware (16MB Flash and 8MB PSRAM), use this configuration in your `platformio.ini`:

```ini
[env:esp32-s3-n16r8]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

; Use custom framework with Bluepad32 integrated BTstack
platform_packages =
    framework-arduinoespressif32 @ https://github.com/maxgerhardt/pio-framework-bluepad32/archive/refs/heads/main.zip

# Hardware Specifics for N16R8 (16MB Flash, 8MB Octal PSRAM)
board_upload.flash_size = 16MB
board_build.partitions = default_16MB.csv
board_build.arduino.memory_type = qio_opi
board_build.flash_mode = qio
board_build.psram_type = opi

build_flags =
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DARDUINO_USB_MODE=1
    -DBOARD_HAS_PSRAM

lib_deps =
    madhephaestus/ESP32Servo @ ^1.1.0
```

### Configuration Notes

- **platform_packages**: Required for Xbox controller support (Bluepad32 integrated into ESP32 Arduino core)
- **board_build.arduino.memory_type = qio_opi**: Enables 8MB PSRAM access on N16R8 variant
- **board_upload.flash_size = 16MB**: Configures full flash capacity usage

## Recommended Components

For the best results in small-scale retrofits (1/24 or 1/32 scale), consider these high-quality components:

### Tank Mode

The **Generic 2S-3S 5A Dual Brushed ESC** is the perfect choice for Tank Mode builds. Its dual-channel output is designed specifically for small 130/180 brushed motors, allowing you to fit everything inside a tight chassis without sacrificing power.

### Car Mode

For **Car Mode** setups with standard motors and steering servos, the **Flipsky Dual Way Brushed ESC** is a highly versatile option. It allows switching between independent and mixed modes physically, providing a great hardware-level fallback for your custom code.

## Getting Started

1. Install PlatformIO IDE in VS Code
2. Clone this repository to your local machine
3. Open the project folder in PlatformIO
4. Configure `platformio.ini` with your specific hardware settings
5. Connect ESP32-S3 via USB-C
6. Flash firmware: `pio run --target upload`
7. Pair Xbox controller (hold pairing button on controller, wait for connection LED)
8. Test both Tank Mode and Car Mode functionality

### Serial Monitor Setup

- **Baud Rate**: 115200
- Useful for monitoring system status, failsafe events, and debugging

## Troubleshooting

| Issue                     | Possible Cause      | Solution                                   |
| ------------------------- | ------------------- | ------------------------------------------ |
| Cannot connect controller | Wrong framework     | Verify platform_packages in platformio.ini |
| Motors not responding     | PWM pin issue       | Check GPIO 1 & 2 wiring                    |
| System hangs              | Watchdog timeout    | Serial shows "System detected as halted"   |
| No PSRAM detected         | Memory config wrong | Verify qio_opi setting in platformio.ini   |

## Support & Contributing

For issues, feature requests, or contributions, please open an issue on the GitHub repository.

---

**License**: See LICENSE file for terms of use
