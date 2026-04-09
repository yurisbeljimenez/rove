# Rove RC Firmware: Deployment & Hardware Integration Guide

This guide provides everything you need to move from code on your computer to a functional, high-performance RC vehicle using the **Rove** firmware.

---

## 🛠 1. Hardware Assembly & Wiring

Proper wiring is critical for both performance and safety. Incorrect power connections can damage the ESP32-S3 or cause unpredictable motor behavior.

### Component Connection Table

| Component             | ESP32-S3 Pin | Function      | Description                                               |
| :-------------------- | :----------- | :------------ | :-------------------------------------------------------- |
| **ESC BEC (5V)**      | `5V / VBUS`  | Power In      | Connect the 5V output from your ESC to power the MCU.     |
| **ESC BEC (GND)**     | `GND`        | Common Ground | Ensure a shared ground between the battery/ESC and ESP32. |
| **Left Motor/ESC**    | `GPIO 1`     | PWM Output    | Controls speed for Tank Mode or Throttle for Car Mode.    |
| **Right Motor/Servo** | `GPIO 2`     | PWM Output    | Controls steering (Car) or Right Motor (Tank).            |

### Wiring Diagram Concept

```text
      [ Battery ]
          │
          ▼
     [ ESC / BEC ] ───────┐
          │               │
          ├─► [ Left Motor/ESC ] ───► GPIO 1
          │
          ├─► [ Right Motor/Servo] ──► GPIO 2
          │
          └─► [ 5V Output ] ────────► ESP32-S3 (VBUS/5V)
                                     [ GND Output ] ──► ESP32-S3 (GND)
```

> **⚠️ Safety Warning:** Always disconnect the battery before making or changing wiring connections. Ensure all connections are secure and insulated to prevent short circuits.

---

## 🎮 2. Bluetooth Pairing Guide

Rove relies on an **Xbox Wireless Controller** (or any XInput gamepad compatible with Bluepad32).

### How to Pair

1. **Power On the Controller:** Turn on your Xbox controller.
2. **Enter Pairing Mode:** Press and hold the small **Pairing Button** on the top of the controller until the Xbox button flashes rapidly.
3. **Wait for Connection:** The ESP32-S3 will automatically detect the controller via Bluepad32.
4. **Confirmation:** Once connected, the controller's LED will turn solid. If it fails to connect, restart the firmware and ensure no other devices are interfering with the Bluetooth signal.

### Troubleshooting Pairing

- **Rapid Flashing vs. Slow Flashing:** Rapid flashing means it is in pairing mode; slow flashing usually means it is searching for a previously paired device.
- **Controller Not Found:** Ensure your controller supports standard Bluetooth (some older Xbox One controllers require a specific hardware revision).

---

## 🚀 3. Software Deployment Workflow

Follow these steps to flash the firmware onto your ESP32-S3.

### Prerequisites

1. **VS Code Installed**
2. **PlatformIO IDE Extension:** Install this via the VS Code Extensions marketplace.
3. **USB-C Cable:** Ensure you are using a data-capable USB cable.

### Flashing Steps

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/your-repo/rove.git
   cd rove
   ```
2. **Configure Hardware:** Open `platformio.ini` and ensure your environment matches your hardware (e.g., `[env:esp32-s3-n16r8]` for 16MB Flash/8MB PSRAM).
3. **Connect ESP32-S3:** Plug the board into your computer via USB.
4. **Build & Upload:**
   - In VS Code, click the **PlatformIO icon** (ant head) on the sidebar.
   - Select **Project Tasks** $\rightarrow$ `esp32-s3-n16r8` $\rightarrow$ **Upload**.
   - _Alternatively, via CLI:_ `pio run --target upload`

### Verification

Once flashing is complete, open the **Serial Monitor** in PlatformIO:

- **Baud Rate:** `115200`
- **What to look for:** You should see the following logs:
  ```text
  [INFO] [MOTOR] Motor control initialized
  [INFO] [INPUT] Input handler initialized
  [INFO] [DRIVE] Drive state machine initialized
  ```
  If you see these messages, your firmware is running successfully and is ready for driving!

---

## 🔍 Troubleshooting Deployment

| Symptese                                  | Possible Cause             | Solution                                                   |
| :---------------------------------------- | :------------------------- | :--------------------------------------------------------- |
| **"Failed to connect to ESP32"**          | Bootloader mode not active | Hold the `BOOT` button on the ESP32 while clicking Upload. |
| **Garbage text in Serial Monitor**        | Wrong Baud Rate            | Ensure monitor is set to `115200`.                         |
| **Motors don't move, but logs look good** | Pin/Wiring mismatch        | Double-check GPIO 1 & 2 connections.                       |
| **Controller connects then disconnects**  | Low Battery / Interference | Charge controller or move away from WiFi routers.          |
