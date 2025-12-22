#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>
#include "config.h"
#include "logger.h"

/**
 * @brief Motor Control class implementing Hardware Abstraction Layer
 * 
 * This class translates mathematical calculations into PWM signals for specific pins,
 * following the HAL (Hardware Abstraction Layer) design pattern.
 */
class MotorControl {
public:
    /**
     * @brief Construct a new MotorControl object
     */
    MotorControl();

    /**
     * @brief Initialize the motor control system
     */
    void begin();

    /**
     * @brief Set car mode parameters
     * 
     * @param throttlePwm PWM value for throttle (1000-2000us)
     * @param steeringPwm PWM value for steering (1000-2000us)
     */
    void setCarMode(int throttlePwm, int steeringPwm);

    /**
     * @brief Set tank mode parameters
     * 
     * @param leftMotorPwm PWM value for left motor (1000-2000us)
     * @param rightMotorPwm PWM value for right motor (1000-2000us)
     */
    void setTankMode(int leftMotorPwm, int rightMotorPwm);

    /**
     * @brief Set all outputs to neutral state
     */
    void setNeutral();

    /**
     * @brief Get singleton instance
     * 
     * @return MotorControl* Pointer to singleton instance
     */
    static MotorControl* getInstance() {
        static MotorControl instance;
        return &instance;
    }

private:
    // PWM output values
    int leftMotorPwm;
    int rightMotorPwm;
    
    // Flag to track if servo library is initialized
    bool servoInitialized;
};

#endif // MOTOR_CONTROL_H
