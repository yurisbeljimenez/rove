#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <Arduino.h>
#include <Bluepad32.h>
#include "config.h"
#include "logger.h"

// Forward declaration
class DriveState;

/**
 * @brief Input Handler class for managing Xbox controller input
 * 
 * This class handles Xbox controller BLE pairing and raw stick data processing
 * following the Observer Pattern design.
 */
class InputHandler {
public:
    /**
     * @brief Construct a new InputHandler object
     */
    InputHandler();

    /**
     * @brief Initialize the input handler
     */
    void begin();

    /**
     * @brief Get current gamepad data
     * 
     * @return const Gamepad* Pointer to current gamepad data
     */
    const Gamepad* getGamepad() const;

    /**
     * @brief Set reference to drive state for mode switching
     * 
     * @param state Pointer to DriveState instance
     */
    void setDriveState(DriveState* state);

    /**
     * @brief Get singleton instance
     * 
     * @return InputHandler* Pointer to singleton instance
     */
    static InputHandler* getInstance() {
        return instance;
    }

private:
    // Static callback functions for Bluepad32
    static void onControllerConnected(const Gamepad* pad);
    static void onControllerDisconnected(const Gamepad* pad);
    static void onGamepadUpdated(const Gamepad* pad);

    // Instance methods for handling updates
    void handleGamepadUpdate(const Gamepad* pad);

    // Static instance for singleton pattern
    static InputHandler* instance;

    // Current gamepad data
    Gamepad currentGamepad;

    // Reference to drive state for mode switching
    DriveState* driveState = nullptr;
};

#endif // INPUT_HANDLER_H