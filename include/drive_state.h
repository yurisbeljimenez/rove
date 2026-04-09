#ifndef DRIVE_STATE_H
#define DRIVE_STATE_H

#include <Arduino.h>
#include "config.h"
#include "logger.h"

// Forward declaration
class InputHandler;

/**
 * @file drive_state.h
 * @brief Manages the vehicle's driving behavior and mode transitions.
 */

/**
 * @class DriveState
 * @brief Implements a Finite State Machine (FSM) to handle different driving modes.
 * 
 * The DriveState class is responsible for interpreting gamepad input and translating it into 
 * appropriate motor commands based on the active mode (Car Mode or Tank Mode). It also 
 * manages transitions between these modes and ensures the vehicle enters a safe state 
 * (FAILSAFE) if control is lost.
 */
class DriveState {
public:
    /**
     * @brief Enumeration of possible driving states/modes for the FSM.
     */
    enum class DriveMode {
        /** Initial state before system is ready */
        INIT,
        /** Standard car steering (servo + single motor) */
        CAR,
        /** Differential steering (twin motors/tank treads) */
        TANK,
        /** Emergency safe state with all outputs neutralized */
        FAILSAFE
    };

    /**
     * @brief Constructs a new DriveState object.
     */
    DriveState();

    /**
     * @brief Initializes the drive state machine and sets the default mode.
     */
    void begin();

    /**
     * @brief Performs one iteration of the FSM update logic.
     * 
     * Processes mode transitions, reads gamepad input via the provided handler,
     * calculates motor outputs based on current mode, and handles failsafe conditions.
     * 
     * @param inputHandler Pointer to the active InputHandler for reading controller data.
     */
    void update(InputHandler* inputHandler);

    /**
     * @brief Signals that a driving mode switch has been requested.
     * 
     * This uses an event-driven approach by setting an internal flag, which is
     * processed during the next `update()` cycle to ensure state consistency.
     */
    void requestModeSwitch();

    /**
     * @brief Executes the transition between driving modes.
     * 
     * Internal FSM method used when a mode switch has been requested.
     */
    void switchMode();

    /**
     * @brief Retrieves the current active driving mode.
     * 
     * @return DriveMode The current state of the machine.
     */
    DriveMode getMode() const;

    /**
     * @brief Immediately sets all motor outputs to their neutral PWM positions.
     * 
     * Used during failsafe activation or when a brake command is received.
     */
    void setNeutral();

    /**
     * @brief Triggers an immediate braking/stopping action.
     * 
     * Sets the vehicle to a neutral state and logs the event.
     */
    void applyBreak();

private:
    /** @brief The current mode of the Finite State Machine. */
    DriveMode currentMode;

    /** @brief Flag indicating that a mode transition is pending for the next update cycle. */
    bool modeSwitchRequested;
};

#endif // DRIVE_STATE_H
