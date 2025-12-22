# Rove Hardware Selection Guide

For your Rove conversions using the **ESP32-S3**, selecting the right hardware depends on whether you prioritize raw speed, balanced performance, or high-torque tank-style maneuverability. Since your firmware supports toggling between **Car** and **Tank** modes, choosing versatile ESCs and motors is key.

## Speed-Focused Conversions

For high-speed builds, you need motors with lower turn counts (for higher RPM) and ESCs with a high burst current to handle the aggressive power delivery.

- **Motor**: The Meus Racing 180 Brushed Motor 44T is an excellent choice. Its **44T** winding is designed for high speed (**26,500 RPM**), and the double ball bearings ensure smooth operation at high velocities.
- **ESC**: A higher-amperage unit like the Spektrum Firma 25A Brushed ESC is recommended. It handles **2S-3S LiPo** setups and provides explosive acceleration, which is necessary for speed-focused chassis.

## Standard/Balanced Performance

These recommendations provide a reliable balance of speed and control, perfect for everyday retrofits where you want a smooth driving experience.

- **Motor**: The HobbyZone Brushed 180 Motor Set is a dependable, high-quality replacement for standard 1/18 or 1/24 scale cars. It offers consistent performance across various terrains.
- **ESC**: The Hobbywing Quicrun 1060 1/10 Brushed ESC is a gold standard for reliability. It features an easy-to-use **jump cap system** for parameter settings and is fully waterproof, making it a "set and forget" choice for standard builds.

## Tank/Differential Style

Tank builds require independent control of two motors. A dual-channel ESC is essential here to keep the footprint small and the wiring simple.

- **Motor**: High-torque motors like the Meus Racing 180 102T are ideal. The **102T** rating provides 30% more torque than standard motors, allowing for powerful differential steering and climbing.
- **ESC**: The Brushed ESC Dual Way Speed Controller from **Flipsky** is specifically designed for this use case. It allows you to switch between **Mixed** and **Independent** modes, perfectly matching your firmware's Tank mode logic while supporting 2S-3S batteries.

## Hardware Summary

| Drive Style  | Recommended Motor    | Recommended ESC        | Battery Support |
| ------------ | -------------------- | ---------------------- | --------------- |
| **Speed**    | Meus Racing 180 44T  | Spektrum Firma 25A     | 2S - 3S LiPo    |
| **Standard** | HobbyZone 180 Set    | Hobbywing Quicrun 1060 | 2S - 3S LiPo    |
| **Tank**     | Meus Racing 180 102T | Flipsky Dual Way 5A    | 2S - 3S LiPo    |

## Additional Considerations

Since you're using the **N16R8 S3** with 8MB of PSRAM, you have the option to add FPV (First Person View) capabilities to your RC vehicle. This would allow you to stream video from a micro-camera directly to a browser over your local network, providing an immersive driving experience.

Would you like me to provide recommendations for micro-cameras and FPV setup components that would work well with your Rove system?
