#include <Arduino.h>

// Include our custom headers
#include "config.h"
#include "input_handler.h"
#include "drive_state.h"
#include "motor_control.h"

// Global instances
InputHandler inputHandler;
DriveState driveState;
MotorControl motorControl;

/**
 * @brief Theory of Operation
 * 
 * This is the main system setup and loop implementation following the 
 * Singleton/Manager pattern for system initialization and RTOS task management.
 * 
 * The system follows these key principles:
 * 1. Finite State Machine (FSM) for driving modes
 * 2. Separation of Concerns between Input, Logic, and Hardware layers
 * 3. Event-Driven callbacks instead of polling for efficiency
 * 4. Non-blocking operation using RTOS tasks
 * 5. Proper logging with standardized format
 */
void setup() {
    // Initialize serial communication for logging
    Serial.begin(115200);
    
    // Initialize components in proper order
    motorControl.begin();
    inputHandler.begin();
    driveState.begin();
    
    // Set up the relationship between components
    inputHandler.setDriveState(&driveState);
    
    Serial.println("[INFO] [MAIN] Rove system initialized successfully");
}

void loop() {
    // Update the drive state based on input
    driveState.update(&inputHandler);
    
    // Small delay to prevent excessive CPU usage
    delay(10);
}
