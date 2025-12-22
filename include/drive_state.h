#ifndef DRIVE_STATE_H
#define DRIVE_STATE_H

#include <Arduino.h>
#include "config.h"
#include "logger.h"

// Forward declaration
class InputHandler;

/**
 * @brief Drive State class implementing Finite State Machine logic
 * 
 * This class manages the state machine for different driving modes (Car Mode, Tank Mode)
 * and performs the arcade or tank mixing math.
 */
class DriveState {
public:
    /**
     * @brief Driving modes
     */
    typedef enum {
        MODE_INIT,
        MODE_CAR,
        MODE_TANK,
        MODE_FAILSAFE
    } DriveMode;

    /**
     * @brief Construct a new DriveState object
     */
    DriveState();

    /**
     * @brief Initialize the drive state machine
     */
    void begin();

    /**
     * @brief Update the drive state based on input
     * 
     * @param inputHandler Reference to input handler for gamepad data
     */
    void update(InputHandler* inputHandler);

    /**
     * @brief Switch between driving modes
     */
    void switchMode();

    /**
     * @brief Get current driving mode
     * 
     * @return DriveMode Current driving mode
     */
    DriveMode getMode() const;

    /**
     * @brief Get neutral state for failsafe
     */
    void setNeutral();

    /**
     * @brief Apply break/stop function
     */
    void applyBreak();

private:
    // Current driving mode
    DriveMode currentMode;

    // Mode switching flag
    bool modeSwitchRequested;
};

#endif // DRIVE_STATE_H
