# Rove

**Rove** is a high-performance, non-blocking RC firmware designed to retrofit toy-grade RC cars with the precision of an **Xbox Wireless Controller**. By leveraging the dual-core power and 8MB of PSRAM on the **ESP32-S3**, Rove delivers a lag-free driving experience that replaces cheap, unresponsive stock transmitters.

## Key Features

Rove transforms your RC car with these powerful capabilities:

- **Dual-Mode Operation**: Seamlessly switch between **Tank Mode** (differential steering for tracked or twin-motor vehicles) and **Car Mode** (standard motor with a steering servo) using the **Y Button**
- **Intuitive Control**: Arcade-style driving using the **Left Stick** - Y-axis controls speed, X-axis handles direction
- **Smart Safety**: Automatic failsafe that sets all motor outputs to neutral when Bluetooth connection to your Xbox controller is lost
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

## Hardware Overview

The ESP32-S3 serves as the central hub for your RC car, keeping the interior clean and organized. Most retrofits will use the **BEC (Battery Eliminator Circuit)** from your ESC to power the board directly.

### Wiring Connections

| Component         | ESP32-S3 Pin | Function                            |
| ----------------- | ------------ | ----------------------------------- |
| **ESC BEC (5V)**  | 5V / VBUS    | Powers the MCU                      |
| **ESC BEC (GND)** | GND          | Shared Ground                       |
| **ESC/Servo 1**   | GPIO 1       | Left Motor (Tank) / Throttle (Car)  |
| **ESC/Servo 2**   | GPIO 2       | Right Motor (Tank) / Steering (Car) |

## PlatformIO Configuration

For optimal performance with your **ESP32-S3 N16R8** hardware (16MB Flash and 8MB PSRAM), use this configuration in your `platformio.ini`:

```ini
[env:esp32-s3-n16r8]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino

board_upload.flash_size = 16MB
board_build.partitions = default_16MB.csv
board_build.arduino.memory_type = qio_opi

build_flags =
    -DBOARD_HAS_PSRAM
    -DARDUINO_USB_CDC_ON_BOOT=1

lib_deps =
    ricardoquesada/Bluepad32 @ ^3.10.1
    madhephaestus/ESP32Servo @ ^1.1.0
```

## Recommended Components

For the best results in small-scale retrofits (1/24 or 1/32 scale), consider these high-quality components:

### Tank Mode

The **Generic 2S-3S 5A Dual Brushed ESC** is the perfect choice for Tank Mode builds. Its dual-channel output is designed specifically for small 130/180 brushed motors, allowing you to fit everything inside a tight chassis without sacrificing power.

### Car Mode

For **Car Mode** setups with standard motors and steering servos, the **Flipsky Dual Way Brushed ESC** is a highly versatile option. It allows switching between independent and mixed modes physically, providing a great hardware-level fallback for your custom code.

## Getting Started

1. Install PlatformIO IDE
2. Configure your `platformio.ini` with the provided settings
3. Connect your ESP32-S3 according to the wiring diagram
4. Flash the firmware and pair with your Xbox controller
5. Test both Tank Mode and Car Mode functionality

## Support & Contributing

For issues, feature requests, or contributions, please open an issue on the GitHub repository.
