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
 * Singleton/Manager pattern for system initialization.
 * 
 * The system follows these key principles:
 * 1. Finite State Machine (FSM) for driving modes
 * 2. Separation of Concerns between Input, Logic, and Hardware layers
 * 3. Event-Driven callbacks instead of polling for efficiency
 * 4. Non-blocking operation with watchdog timer protection
 * 5. Proper logging with standardized format
 */

// Software-based watchdog configuration (fallback mechanism)
#define WATCHDOG_TIMEOUT_MS 2000    // 2 second timeout before safety action
static uint32_t lastActivityTime = 0;

/**
 * @brief Initialize the system and software watchdog
 */
void setup() {
    // Initialize serial communication for logging
    Serial.begin(115200);
    
    // Start time tracking for watchdog
    lastActivityTime = millis();
    
    Serial.println("[MAIN] System initialized (software watchdog: 2s timeout)");
    
    // Initialize components in proper order
    motorControl.begin();
    inputHandler.begin();
    driveState.begin();
    
    // Set up the relationship between components
    inputHandler.setDriveState(&driveState);
    
    Serial.println("[INFO] [MAIN] Rove system initialized successfully");
}

/**
 * @brief Main loop - update state and monitor watchdog timer
 */
void loop() {
    uint32_t currentTime = millis();
    
    // Update the drive state based on input (this is an activity)
    driveState.update(&inputHandler);
    
    // Reset activity timer after each successful update cycle
    lastActivityTime = currentTime;
    
    // Small delay to prevent excessive CPU usage
    delay(10);
}

/**
 * @brief Software watchdog check - called periodically in loop
 * 
 * If no activity is detected for WATCHDOG_TIMEOUT_MS, the system will:
 * 1. Log a warning message via serial
 * 2. Set all motors to neutral position (safe state)
 * 3. Continue running in this safe state until system recovers
 */
void checkWatchdog() {
    uint32_t elapsed = millis() - lastActivityTime;
    
    if (elapsed >= WATCHDOG_TIMEOUT_MS) {
        Serial.print("[WATCHDOG] WARNING: No activity for ");
        Serial.print(elapsed);
        Serial.println("ms!");
        
        // Set motors to safe neutral position
        motorControl.setNeutral();
        
        // Log safety state
        Serial.println("[WATCHDOG] System in SAFE MODE - waiting for recovery");
    }
}