#include <Arduino.h>
#include "config.h"
#include "input_handler.h"
#include "drive_state.h"
#include "motor_control.h"

// Global instances
InputHandler inputHandler;
DriveState driveState;
MotorControl motorControl;

/**
 * @file main.cpp
 * @brief Main entry point for the Rove RC firmware.
 * 
 * This file initializes the core components (MotorControl, InputHandler, DriveState)
 * and runs the main control loop.
 */

/**
 * @brief System setup function called once at startup.
 * 
 * Initializes serial communication, hardware abstraction layers, input handling,
 * and the driving state machine.
 */
void setup() {
    Serial.begin(115200);
    
    motorControl.begin();
    inputHandler.begin();
    driveState.begin();
    
    // Link input handler to the drive state for event-driven mode switching
    inputHandler.setDriveState(&driveState);
}

/**
 * @brief Main execution loop.
 * 
 * Continuously updates the driving state machine, which in turn processes
 * controller inputs and updates motor outputs.
 */
void loop() {
    driveState.update(&inputHandler);
    delay(10);
}
