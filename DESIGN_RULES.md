# Rove Design Rules & Architecture Guidelines

To ensure Rove is built with professional-grade stability and is easy for an AI or human coding agent to navigate, we will adopt a Finite State Machine (FSM) architecture and a Modular Object-Oriented file structure.

## 1. Architectural Principles

The project will follow these core design rules to ensure it remains "non-blocking" and easy to debug:

### Finite State Machine (FSM)

The system's behavior is dictated by explicit states (e.g., INIT, CAR_MODE, TANK_MODE, FAILSAFE). This prevents "impossible" behaviors, like the car driving while the controller is disconnected.

### Separation of Concerns

- **Input Layer**: Handles Xbox BLE pairing and raw stick data
- **Logic Layer**: Performs the "Arcade" or "Tank" mixing math
- **Hardware Abstraction Layer (HAL)**: Translates math into PWM signals for specific pins
- **Event-Driven (Callbacks)**: Instead of constantly checking "is the button pressed?", the code reacts to updates from the Bluepad32 callback, saving CPU cycles for the dual-core S3

## 2. File Organization & Naming Rules

We will use PlatformIO's standard structure. File names will be in snake_case, while Classes will use PascalCase.

| File Name         | Responsibility                         | Key Pattern       |
| ----------------- | -------------------------------------- | ----------------- |
| main.cpp          | System setup, RTOS task management     | Singleton/Manager |
| input_handler.cpp | Xbox Controller logic & BLE callbacks  | Observer Pattern  |
| drive_state.cpp   | The State Machine logic (Mixer math)   | FSM               |
| motor_control.cpp | Direct PWM/Servo output handling       | HAL               |
| config.h          | Pin definitions and hardware constants | Global Constants  |

## 3. Project Structure

The project follows a standard PlatformIO structure with:

- `src/` directory for source code files
- `include/` directory for header files
- `lib/` directory for external libraries

All header files should be included using the proper include path format as expected by PlatformIO.

## 3. Documentation & Coding Patterns

To make the code readable for an agent, every module must include a "Theory of Operation" header.

### Self-Documenting Names

Variables must include units where applicable (e.g., throttlePercent, steeringPulseMs).

### The "Neutral" Failsafe

Every output function must have a stop() or neutral() method that is the default state during any error.

### No Magic Numbers

All values like 1500 (Neutral PWM) must be defined in config.h as PWM_NEUTRAL_US.

## 4. Additional Design Considerations

### Non-blocking Operation

The system is designed to be completely non-blocking, ensuring that all operations can be performed asynchronously without blocking the main execution thread.

### Debuggability

All major components are designed with debugging in mind, including clear state transitions and error handling mechanisms.

### Maintainability

The modular approach ensures that individual components can be updated or modified without affecting the entire system.

### Logger Pattern

When injecting a logger into any component, it must follow a standard pattern using log levels. Log messages should be formatted as:

"[LOG_LEVEL] [COMPONENT_NAME] Message content"

For example:

- "[INFO] [DRIVE] Mode switched to TANK_MODE"
- "[DEBUG] [INPUT] Controller data received"
- "[ERROR] [MOTOR] PWM signal out of range"

This standardization facilitates debugging and development by clearly identifying the log level, component source, and message content.
