#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <Arduino.h>
#include "config.h"
#include "logger.h"
#include <Bluepad32.h>  // Include for Controller type definition

// Forward declaration - only needed if we don't include Bluepad32.h above
class DriveState;

/**
 * @brief Input Handler class for managing Xbox controller input
 * 
 * This class handles Xbox controller connection and raw stick data processing.
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
     * @brief Get current controller data
     * 
     * @return const Controller* Pointer to current controller data (from Bluepad32)
     */
    const Controller* getGamepad() const;

    /**
     * @brief Set reference to drive state for mode switching
     * 
     * @param state Pointer to DriveState instance
     */
    void setDriveState(DriveState* state);

    /**
     * @brief Get singleton instance for use in callbacks
     * 
     * @return InputHandler* Pointer to singleton instance, nullptr if not initialized
     */
    static InputHandler* getInstance();

    /**
     * @brief Handle gamepad update (polling-based)
     * 
     * Call this from the main loop to process controller input.
     */
    void handleGamepadUpdate();

private:
    // Static instance for singleton pattern
    static InputHandler* instance;

    // Current controller data - using Controller type from Bluepad32.h
    Controller currentController;

    // Reference to drive state for mode switching
    DriveState* driveState = nullptr;
    
    // Friend declaration for callback function access
    friend void onControllerData(uni_hid_device_t*, uni_controller_t*);
};

// Forward declaration of the static callback
void onControllerData(uni_hid_device_t* d, uni_controller_t* ctl);

#endif // INPUT_HANDLER_H
