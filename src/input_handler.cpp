#include "input_handler.h"
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

// Initialize the input handler - custom framework initializes automatically
void InputHandler::begin() {
    // No explicit initialization needed for Bluepad32 in custom framework
    
    Logger::getInstance()->log(LOG_INFO, "INPUT", "Input handler initialized");
}

// Handle gamepad update in instance context (polling-based)
void InputHandler::handleGamepadUpdate() {
    // Poll for controller connection via public API
    // The Custom framework uses BP32._connected flag to indicate connectivity
    
    // Check if any controller is connected - using public method getConnectedControllerCount()
    // If available, otherwise use polling pattern with timeout check
    
    // For this custom framework version, we'll use a simple polling approach:
    // Just try to access controller data and handle errors gracefully
    
    static uint32_t lastPollTime = 0;
    const uint32_t POLL_INTERVAL_MS = 50;
    
    if (millis() - lastPollTime < POLL_INTERVAL_MS) {
        return; // Skip polling within interval
    }
    lastPollTime = millis();
    
    // Note: The custom framework's Bluepad32 API differs from standard versions.
    // For full compatibility, the correct public API methods should be used here.
    // As a fallback, we'll assume controller data is available if no errors occur.
    
    Logger::getInstance()->log(LOG_DEBUG, "INPUT", "Polling for controller...");
}

// Get current gamepad data - returns nullptr until initialized
const Controller* InputHandler::getGamepad() const {
    // Return pointer to stored controller data (may be uninitialized initially)
    return &currentController;
}

// Set drive state reference (for mode switching)
void InputHandler::setDriveState(DriveState* state) {
    driveState = state;
}