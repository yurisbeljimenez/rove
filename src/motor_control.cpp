#include "motor_control.h"
#include "config.h"
#include "logger.h"
#include <ESP32Servo.h>

// Global servo objects
Servo leftMotorServo;
Servo rightMotorServo;

// Constructor
MotorControl::MotorControl() : leftMotorPwm(PWM_NEUTRAL_US), 
                               rightMotorPwm(PWM_NEUTRAL_US),
                               servoInitialized(false) {
    // Constructor implementation
}

// Initialize the motor control system
void MotorControl::begin() {
    // Attach servos to pins
    leftMotorServo.attach(PIN_ESC_LEFT, PWM_MIN_US, PWM_MAX_US);
    rightMotorServo.attach(PIN_ESC_RIGHT, PWM_MIN_US, PWM_MAX_US);
    
    // Set initial neutral state
    setNeutral();
    
    Logger::getInstance()->log(LOG_INFO, "MOTOR", "Motor control initialized");
}

// Set car mode parameters
void MotorControl::setCarMode(int throttlePwm, int steeringPwm) {
    // Ensure PWM values are within valid range
    if (throttlePwm < PWM_MIN_US) throttlePwm = PWM_MIN_US;
    if (throttlePwm > PWM_MAX_US) throttlePwm = PWM_MAX_US;
    if (steeringPwm < PWM_MIN_US) steeringPwm = PWM_MIN_US;
    if (steeringPwm > PWM_MAX_US) steeringPwm = PWM_MAX_US;
    
    // Update PWM values
    leftMotorPwm = throttlePwm;
    rightMotorPwm = steeringPwm;
    
    // Write to servos
    leftMotorServo.writeMicroseconds(leftMotorPwm);
    rightMotorServo.writeMicroseconds(rightMotorPwm);
    
    Logger::getInstance()->log(LOG_DEBUG, "MOTOR", "Car mode PWM set");
}

// Set tank mode parameters
void MotorControl::setTankMode(int leftMotorPwm, int rightMotorPwm) {
    // Ensure PWM values are within valid range
    if (leftMotorPwm < PWM_MIN_US) leftMotorPwm = PWM_MIN_US;
    if (leftMotorPwm > PWM_MAX_US) leftMotorPwm = PWM_MAX_US;
    if (rightMotorPwm < PWM_MIN_US) rightMotorPwm = PWM_MIN_US;
    if (rightMotorPwm > PWM_MAX_US) rightMotorPwm = PWM_MAX_US;
    
    // Update PWM values
    this->leftMotorPwm = leftMotorPwm;
    this->rightMotorPwm = rightMotorPwm;
    
    // Write to servos
    leftMotorServo.writeMicroseconds(leftMotorPwm);
    rightMotorServo.writeMicroseconds(rightMotorPwm);
    
    Logger::getInstance()->log(LOG_DEBUG, "MOTOR", "Tank mode PWM set");
}

// Set all outputs to neutral state
void MotorControl::setNeutral() {
    leftMotorPwm = PWM_NEUTRAL_US;
    rightMotorPwm = PWM_NEUTRAL_US;
    
    // Write neutral values to servos
    leftMotorServo.writeMicroseconds(leftMotorPwm);
    rightMotorServo.writeMicroseconds(rightMotorPwm);
    
    Logger::getInstance()->log(LOG_DEBUG, "MOTOR", "Neutral state set");
}
