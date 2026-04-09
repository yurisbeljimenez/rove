#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <Arduino.h>
#include "config.h"
#include "logger.h"
#include <Bluepad32.h>

// Forward declaration
class DriveState;

/**
 * @file input_handler.h
 * @brief Manages Xbox controller connectivity and input processing via Bluepad32.
 */

/**
 * @class InputHandler
 * @brief Singleton class that manages Xbox controller input and Bluepad32 integration.
 * 
 * This class acts as the bridge between the raw Bluetooth gamepad data provided by 
 * Bluepad32 and the Rove firmware's driving logic. It handles connection events,
 * button presses (mode switching, braking), and provides access to stick axes.
 */
class InputHandler {
public:
    /**
     * @brief Constructs a new InputHandler instance.
     * 
     * Initializes the singleton pointer if this is the first instance created.
     */
    InputHandler();

    /**
     * @brief Initializes the input handler and prepares internal buffers.
     * 
     * Should be called once during system setup.
     */
    void begin();

    /**
     * @brief Retrieves the most recent gamepad data snapshot.
     * 
     * @return const Controller* A pointer to the current controller state. 
     *         Returns nullptr if no controller is currently connected/available.
     */
    const Controller* getGamepad() const;

    /**
     * @brief Links the input handler to a DriveState instance.
     * 
     * This allows the InputHandler to trigger mode switches and braking actions
     * directly on the state machine.
     * 
     * @param state Pointer to the active DriveState object.
     */
    void setDriveState(DriveState* state);

    /**
     * @brief Returns the singleton instance of the InputHandler.
     * 
     * Required for access within Bluepad32's static callback functions.
     * 
     * @return InputHandler* The global singleton instance.
     */
    static InputHandler* getInstance();

    /**
     * @brief Processes gamepad input updates.
     * 
     * Currently a placeholder for future polling-based logic, as the system 
     * primarily uses event-driven callbacks from Bluepad32.
     */
    void handleGamepadUpdate();

private:
    /** @brief Static pointer to the single global instance of InputHandler. */
    static InputHandler* instance;

    /** @brief Local storage for the latest controller data received via callback. */
    Controller currentController;

    /** @brief Pointer to the drive state machine for triggering events. */
    DriveState* driveState = nullptr;
    
    /** @brief Grants access to private members for the Bluepad32 callback function. */
    friend void onControllerData(uni_hid_device_t*, uni_controller_t*);
};

/**
 * @brief Bluepad32 event callback triggered when controller data changes.
 * 
 * This is a low-level callback that processes raw HID reports and translates
 * them into Rove-specific actions like mode switching or braking.
 * 
 * @param d Pointer to the HID device context.
 * @param ctl Pointer to the controller data structure provided by Bluepad32.
 */
void onControllerData(uni_hid_device_t* d, uni_controller_t* ctl);

#endif // INPUT_HANDLER_H