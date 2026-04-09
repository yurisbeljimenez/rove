#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>
#include "config.h"
#include "logger.h"

/**
 * @file motor_control.h
 * @brief Provides a Hardware Abstraction Layer (HAL) for motor and servo control.
 */

/**
 * @class MotorControl
 * @brief Implements the Hardware Abstraction Layer for motor and steering control.
 * 
 * This class translates high-level driving commands (Car Mode or Tank Mode) into 
 * low-level PWM signals sent to the ESCs and servos via specific GPIO pins.
 * It follows the Singleton design pattern to ensure unified hardware access.
 */
class MotorControl {
public:
    /**
     * @brief Constructs a new MotorControl object.
     */
    MotorControl();

    /**
     * @brief Initializes the motor control system and attaches PWM pins.
     * 
     * Should be called once during system setup to prepare the ESCs/Servos.
     */
    void begin();

    /**
     * @brief Configures outputs for Standard Car Mode.
     * 
     * Maps throttle and steering commands to the respective PWM pins.
     * 
     * @param throttlePwm Target PWM value for the throttle ESC (typically 1000-2000us).
     * @param steeringPwm Target PWM value for the steering servo (typically 1000-2000us).
     */
    void setCarMode(int throttlePwm, int steeringPwm);

    /**
     * @brief Configures outputs for Tank/Differential Mode.
     * 
     * Maps independent speed commands to the left and right motors.
     * 
     * @param leftMotorPwm Target PWM value for the left motor (1000-2000us).
     * @param rightMotorPwm Target PWM value for the right motor (1000-2000us).
     */
    void setTankMode(int leftMotorPwm, int rightMotorPwm);

    /**
     * @brief Immediately sets all motor/servo outputs to neutral.
     * 
     * Used for safety failsafe and braking procedures.
     */
    void setNeutral();

    /**
     * @brief Retrieves the singleton instance of MotorControl.
     * 
     * @return MotorControl* A pointer to the global MotorControl instance.
     */
    static MotorControl* getInstance() {
        static MotorControl instance;
        return &instance;
    }

private:
    /** @brief Current PWM duty cycle for the left motor/servo output. */
    int leftMotorPwm;
    /** @brief Current PWM duty cycle for the right motor/servo output. */
    int rightMotorPwm;
};

#endif // MOTOR_CONTROL_H
