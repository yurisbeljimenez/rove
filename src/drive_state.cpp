#include "drive_state.h"
#include "input_handler.h"  // Include for InputHandler type and Controller definition via Bluepad32.h
#include "motor_control.h"
#include "config.h"
#include "logger.h"
#include <Arduino.h>

// Constructor
DriveState::DriveState() : currentMode(MODE_INIT), modeSwitchRequested(false) {
    // Constructor implementation
}

// Initialize the drive state machine
void DriveState::begin() {
    currentMode = MODE_CAR; // Default to car mode
    Logger::getInstance()->log(LOG_INFO, "DRIVE", "Drive state machine initialized");
}

// Request a mode switch (event-driven approach)
void DriveState::requestModeSwitch() {
    if (currentMode != MODE_FAILSAFE) {
        modeSwitchRequested = true;
        Logger::getInstance()->log(LOG_DEBUG, "DRIVE", "Mode switch requested");
    } else {
        Logger::getInstance()->log(LOG_WARN, "DRIVE", "Cannot switch modes while in failsafe");
    }
}

// Update the drive state based on input
void DriveState::update(InputHandler* inputHandler) {
    // Handle mode switching if requested (FSM pattern - process events in update cycle)
    if (modeSwitchRequested) {
        switchMode();
        modeSwitchRequested = false;
    }

    // Get current gamepad data from InputHandler
    const Controller* pad = inputHandler->getGamepad();

    // Handle failsafe condition - controller may not be connected yet
    if (pad == nullptr) {
        if (currentMode != MODE_FAILSAFE) {
            currentMode = MODE_FAILSAFE;
            Logger::getInstance()->log(LOG_WARN, "DRIVE", "Controller disconnected - entering failsafe");
        }
        setNeutral();
        return;
    }

    // Process input based on current mode (no debug log per update to reduce overhead)
    switch (currentMode) {
        case MODE_CAR:
            // Car mode: standard motor with steering servo
            // Y-axis controls throttle, X-axis controls steering
            {
                int32_t throttle = pad->axisY();  // Method call - returns -32768 to 32767
                int32_t steering = pad->axisX();  // Method call - returns -32768 to 32767

                // Convert to PWM values (map handles range clamping)
                int throttlePwm = map(throttle, -32768, 32767, PWM_MIN_US, PWM_MAX_US);
                int steeringPwm = map(steering, -32768, 32767, PWM_MIN_US, PWM_MAX_US);

                // Send to motor control
                MotorControl::getInstance()->setCarMode(throttlePwm, steeringPwm);
            }
            break;

        case MODE_TANK:
            // Tank mode: differential steering for tracked vehicles
            {
                int32_t leftMotor = pad->axisY();   // Method call - returns -32768 to 32767
                int32_t rightMotor = pad->axisY();  // Same value, use axisX() for rotation if needed

                // Convert to PWM values (map handles range clamping)
                int leftPwm = map(leftMotor, -32768, 32767, PWM_MIN_US, PWM_MAX_US);
                int rightPwm = map(rightMotor, -32768, 32767, PWM_MIN_US, PWM_MAX_US);

                // Send to motor control
                MotorControl::getInstance()->setTankMode(leftPwm, rightPwm);
            }
            break;

        case MODE_FAILSAFE:
            // Failsafe: set all outputs to neutral
            setNeutral();
            break;

        default:
            // Unknown mode - enter failsafe
            currentMode = MODE_FAILSAFE;
            setNeutral();
            break;
    }
}

// Switch between driving modes (internal use by FSM)
void DriveState::switchMode() {
    switch (currentMode) {
        case MODE_CAR:
            currentMode = MODE_TANK;
            Logger::getInstance()->log(LOG_INFO, "DRIVE", "Switched to TANK_MODE");
            break;
        case MODE_TANK:
            currentMode = MODE_CAR;
            Logger::getInstance()->log(LOG_INFO, "DRIVE", "Switched to CAR_MODE");
            break;
        default:
            currentMode = MODE_CAR;
            Logger::getInstance()->log(LOG_INFO, "DRIVE", "Switched to CAR_MODE (default)");
            break;
    }
}

// Get current driving mode
DriveState::DriveMode DriveState::getMode() const {
    return currentMode;
}

// Set neutral state for failsafe
void DriveState::setNeutral() {
    MotorControl::getInstance()->setNeutral();
}

// Apply break/stop function
void DriveState::applyBreak() {
    Logger::getInstance()->log(LOG_INFO, "DRIVE", "Brake activated");
    setNeutral();
}