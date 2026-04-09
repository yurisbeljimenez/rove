#include "motor_control.h"
#include "config.h"
#include "logger.h"
#include <ESP32Servo.h>

/** @brief Global servo objects for motor control */
Servo leftMotorServo;
Servo rightMotorServo;

/**
 * @brief Constructs a new MotorControl object.
 * 
 * Initializes PWM values to neutral (1500us).
 */
MotorControl::MotorControl() : leftMotorPwm(PWM_NEUTRAL_US), 
                               rightMotorPwm(PWM_NEUTRAL_US) {}

/**
 * @brief Initializes the motor control system and attaches PWM pins.
 * 
 * Configures the ESP32Servo objects for the specified GPIO pins and sets outputs to neutral.
 */
void MotorControl::begin() {
    leftMotorServo.attach(PIN_ESC_LEFT, PWM_MIN_US, PWM_MAX_US);
    rightMotorServo.attach(PIN_ESC_RIGHT, PWM_MIN_US, PWM_MAX_US);
    setNeutral();
    
    Logger::getInstance()->log(LogLevel::INFO, "MOTOR", "Motor control initialized");
}

/**
 * @brief Configures outputs for Standard Car Mode.
 * 
 * @param throttlePwm Target PWM value for the throttle ESC (1000-2000us).
 * @param steeringPwm Target PWM value for the steering servo (1000-2000us).
 */
void MotorControl::setCarMode(int throttlePwm, int steeringPwm) {
    // Values are expected to be within PWM_MIN_US and PWM_MAX_US range from DriveState
    leftMotorPwm = throttlePwm;
    rightMotorPwm = steeringPwm;
    
    leftMotorServo.writeMicroseconds(leftMotorPwm);
    rightMotorServo.writeMicroseconds(rightMotorPwm);
}

/**
 * @brief Configures outputs for Tank/Differential Mode.
 * 
 * @param leftMotorPwm Target PWM value for the left motor (1000-2000us).
 * @param rightMotorPwm Target PWM value for the right motor (1000-2000us).
 */
void MotorControl::setTankMode(int leftMotorPwm, int rightMotorPwm) {
    // Values are expected to be within PWM_MIN_US and PWM_MAX_US range from DriveState
    this->leftMotorPwm = leftMotorPwm;
    this->rightMotorPwm = rightMotorPwm;
    
    leftMotorServo.writeMicroseconds(this->leftMotorPwm);
    rightMotorServo.writeMicroseconds(this->rightMotorPwm);
}

/**
 * @brief Immediately sets all motor/servo outputs to neutral (1500us).
 */
void MotorControl::setNeutral() {
    leftMotorPwm = PWM_NEUTRAL_US;
    rightMotorPwm = PWM_NEUTRAL_US;
    
    leftMotorServo.writeMicroseconds(leftMotorPwm);
    rightMotorServo.writeMicroseconds(rightMotorPwm);
}
