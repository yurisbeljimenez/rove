#include "drive_state.h"
#include "input_handler.h"
#include "motor_control.h"
#include "config.h"
#include "logger.h"
#include <Arduino.h>

/**
 * @brief Apply deadzone to analog stick value.
 * 
 * Removes small input values near center that may be caused by drift.
 * 
 * @param value Raw axis value (-32768 to 32767).
 * @return int32_t Deadzone-adjusted value.
 */
static int32_t applyDeadzone(int32_t value) {
    float normalized = (float)value / 32767.0f;
    
    if (fabs(normalized) < AXIS_DEADZONE_THRESHOLD) {
        return 0;
    }
    
    // Scale back to full range, preserving sign
    float adjusted = normalized;
    if (adjusted >= 0.0f) {
        adjusted = (adjusted - AXIS_DEADZONE_THRESHOLD) / (1.0f - AXIS_DEADZONE_THRESHOLD);
    } else {
        adjusted = (adjusted + AXIS_DEADZONE_THRESHOLD) / (1.0f - AXIS_DEADZONE_THRESHOLD);
    }
    
    return static_cast<int32_t>(adjusted * 32767.0f);
}

/**
 * @brief Constructs a new DriveState object.
 */
DriveState::DriveState() : currentMode(DriveState::DriveMode::INIT), modeSwitchRequested(false) {}

/**
 * @brief Initializes the drive state machine and sets the default mode.
 */
void DriveState::begin() {
    currentMode = DriveState::DriveMode::CAR; // Default to car mode
    Logger::getInstance()->log(LogLevel::INFO, "DRIVE", "Drive state machine initialized");
}

/**
 * @brief Signals that a driving mode switch has been requested.
 */
void DriveState::requestModeSwitch() {
    if (currentMode != DriveState::DriveMode::FAILSAFE) {
        modeSwitchRequested = true;
        Logger::getInstance()->log(LogLevel::DEBUG, "DRIVE", "Mode switch requested");
    } else {
        Logger::getInstance()->log(LogLevel::WARN, "DRIVE", "Cannot switch modes while in failsafe");
    }
}

/**
 * @brief Performs one iteration of the FSM update logic.
 * 
 * Processes mode transitions, reads gamepad input via the provided handler,
 * calculates motor outputs based on current mode, and handles failsafe conditions.
 * 
 * @param inputHandler Pointer to the active InputHandler for reading controller data.
 */
void DriveState::update(InputHandler* inputHandler) {
    // Handle mode switching if requested (FSM pattern)
    if (modeSwitchRequested) {
        switchMode();
        modeSwitchRequested = false;
    }

    // Get current gamepad data from InputHandler
    const Controller* pad = inputHandler->getGamepad();

    // Handle failsafe condition - controller may not be connected yet
    if (pad == nullptr) {
        if (currentMode != DriveState::DriveMode::FAILSAFE) {
            currentMode = DriveState::DriveMode::FAILSAFE;
            Logger::getInstance()->log(LogLevel::WARN, "DRIVE", "Controller disconnected - entering failsafe");
        }
        setNeutral();
        return;
    }

    // Process input based on current mode
    switch (currentMode) {
        case DriveState::DriveMode::CAR:
            // Car mode: Y-axis = throttle, X-axis = steering
            {
                int32_t throttle = pad->axisY();
                int32_t steering = pad->axisX();

                // Apply deadzone to remove drift near center position
                throttle = applyDeadzone(throttle);
                steering = applyDeadzone(steering);

                // Convert to PWM values (map handles range clamping)
                int throttlePwm = map(throttle, -32768, 32767, PWM_MIN_US, PWM_MAX_US);
                int steeringPwm = map(steering, -32768, 32767, PWM_MIN_US, PWM_MAX_US);

                // Send to motor control
                MotorControl::getInstance()->setCarMode(throttlePwm, steeringPwm);
            }
            break;

        case DriveState::DriveMode::TANK:
            // Tank mode: Y-axis = forward/rev, X-axis = turn (differential)
            {
                int32_t leftMotor = pad->axisY();
                int32_t rightMotor = pad->axisY();

                // Apply deadzone to remove drift near center position
                leftMotor = applyDeadzone(leftMotor);
                rightMotor = applyDeadzone(rightMotor);

                // Convert to PWM values (map handles range clamping)
                int leftPwm = map(leftMotor, -32768, 32767, PWM_MIN_US, PWM_MAX_US);
                int rightPwm = map(rightMotor, -32768, 32767, PWM_MIN_US, PWM_MAX_US);

                // Send to motor control
                MotorControl::getInstance()->setTankMode(leftPwm, rightPwm);
            }
            break;

        case DriveState::DriveMode::FAILSAFE:
            // Failsafe: set all outputs to neutral
            setNeutral();
            break;

        default:
            // Unknown mode - enter failsafe
            currentMode = DriveState::DriveMode::FAILSAFE;
            setNeutral();
            break;
    }
}

/**
 * @brief Executes the transition between driving modes.
 */
void DriveState::switchMode() {
    switch (currentMode) {
        case DriveState::DriveMode::CAR:
            currentMode = DriveState::DriveMode::TANK;
            Logger::getInstance()->log(LogLevel::INFO, "DRIVE", "Switched to TANK_MODE");
            break;
        case DriveState::DriveMode::TANK:
            currentMode = DriveState::DriveMode::CAR;
            Logger::getInstance()->log(LogLevel::INFO, "DRIVE", "Switched to CAR_MODE");
            break;
        default:
            currentMode = DriveState::DriveMode::CAR;
            Logger::getInstance()->log(LogLevel::INFO, "DRIVE", "Switched to CAR_MODE (default)");
            break;
    }
}

/**
 * @brief Retrieves the current active driving mode.
 * 
 * @return DriveMode The current state of the machine.
 */
DriveState::DriveMode DriveState::getMode() const {
    return currentMode;
}

/**
 * @brief Immediately sets all motor outputs to their neutral PWM positions.
 */
void DriveState::setNeutral() {
    MotorControl::getInstance()->setNeutral();
}

/**
 * @brief Triggers an immediate braking/stopping action.
 */
void DriveState::applyBreak() {
    Logger::getInstance()->log(LogLevel::INFO, "DRIVE", "Brake activated");
    setNeutral();
}
