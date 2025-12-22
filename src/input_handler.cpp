#include "input_handler.h"
#include "config.h"
#include <Bluepad32.h>
#include <Arduino.h>

// Static instance for singleton pattern
InputHandler* InputHandler::instance = nullptr;

// Constructor
InputHandler::InputHandler() {
    if (instance == nullptr) {
        instance = this;
    }
}

// Initialize the input handler
void InputHandler::begin() {
    // Initialize Bluepad32
    BP32.begin();
    
    // Set up callback functions
    BP32.setControllerConnectedCallback(onControllerConnected);
    BP32.setControllerDisconnectedCallback(onControllerDisconnected);
    BP32.setGamepadCallback(onGamepadUpdated);
    
    logMessage(LOG_INFO, "INPUT", "Input handler initialized");
}

// Static callback for controller connection
void InputHandler::onControllerConnected(const Gamepad* pad) {
    logMessage(LOG_INFO, "INPUT", "Controller connected");
}

// Static callback for controller disconnection
void InputHandler::onControllerDisconnected(const Gamepad* pad) {
    logMessage(LOG_WARN, "INPUT", "Controller disconnected");
}

// Static callback for gamepad updates
void InputHandler::onGamepadUpdated(const Gamepad* pad) {
    if (instance != nullptr) {
        instance->handleGamepadUpdate(pad);
    }
}

// Handle gamepad update in instance context
void InputHandler::handleGamepadUpdate(const Gamepad* pad) {
    // Store the gamepad data
    memcpy(&currentGamepad, pad, sizeof(Gamepad));
    
    // Check for mode switch button (Y button)
    if (pad->pressedButtons & GAMEPAD_BUTTON_Y) {
        // Notify the drive state machine to switch mode
        if (driveState != nullptr) {
            driveState->switchMode();
        }
    }
    
    // Check for break button (B button)
    if (pad->pressedButtons & GAMEPAD_BUTTON_B) {
        // Notify the drive state machine to apply break
        if (driveState != nullptr) {
            driveState->applyBreak();
        }
    }
    
    // Log the update
    logMessage(LOG_DEBUG, "INPUT", "Gamepad data updated");
}

// Get current gamepad data
const Gamepad* InputHandler::getGamepad() const {
    return &currentGamepad;
}

// Set drive state reference (for mode switching)
void InputHandler::setDriveState(DriveState* state) {
    driveState = state;
}

// Log message helper function
void InputHandler::logMessage(LogLevel level, const char* component, const char* message) {
    // This is a simplified logging approach - in a real implementation,
    // this would use a proper logging system
    switch (level) {
        case LOG_DEBUG:
            Serial.printf("[DEBUG] [%s] %s\n", component, message);
            break;
        case LOG_INFO:
            Serial.printf("[INFO] [%s] %s\n", component, message);
            break;
        case LOG_WARN:
            Serial.printf("[WARN] [%s] %s\n", component, message);
            break;
        case LOG_ERROR:
            Serial.printf("[ERROR] [%s] %s\n", component, message);
            break;
    }
}
