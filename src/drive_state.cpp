#include "drive_state.h"
#include "input_handler.h"
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

// Update the drive state based on input
void DriveState::update(InputHandler* inputHandler) {
    // Handle mode switching if requested
    if (modeSwitchRequested) {
        switchMode();
        modeSwitchRequested = false;
    }
    
    // Get current gamepad data
    const Gamepad* pad = inputHandler->getGamepad();
    
    // Handle failsafe condition (controller disconnected)
    if (pad == nullptr) {
        if (currentMode != MODE_FAILSAFE) {
            currentMode = MODE_FAILSAFE;
            Logger::getInstance()->log(LOG_WARN, "DRIVE", "Controller disconnected - entering failsafe");
        }
        setNeutral();
        return;
    }
    
    // Process input based on current mode
    switch (currentMode) {
        case MODE_CAR:
            // Car mode: standard motor with steering servo
            // Y-axis controls throttle, X-axis controls steering
            {
                int throttle = pad->axisY;  // -32768 to 32767
                int steering = pad->axisX;  // -32768 to 32767
                
                // Convert to PWM values
                int throttlePwm = map(throttle, -32768, 32767, PWM_MIN_US, PWM_MAX_US);
                int steeringPwm = map(steering, -32768, 32767, PWM_MIN_US, PWM_MAX_US);
                
                // Send to motor control
                MotorControl::getInstance()->setCarMode(throttlePwm, steeringPwm);
            }
            break;
            
        case MODE_TANK:
            // Tank mode: differential steering for tracked vehicles
            // Y-axis controls left/right motors independently
            {
                int leftMotor = pad->axisY;   // -32768 to 32767
                int rightMotor = pad->axisY;  // -32768 to 32767
                
                // For differential steering, we might want different values
                // Using axisY for both but in opposite directions for tank
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
    
    Logger::getInstance()->log(LOG_DEBUG, "DRIVE", "Drive state updated");
}

// Switch between driving modes
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
    Logger::getInstance()->log(LOG_DEBUG, "DRIVE", "Neutral state set");
}

// Apply break/stop function
void DriveState::applyBreak() {
    Logger::getInstance()->log(LOG_INFO, "DRIVE", "Break applied");
    setNeutral();
}
