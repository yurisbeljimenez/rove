#include "motor_control.h"
#include "config.h"
#include "logger.h"
#include <ESP32Servo.h>

// Global servo objects
Servo leftMotorServo;
Servo rightMotorServo;

// Constructor
MotorControl::MotorControl() : leftMotorPwm(PWM_NEUTRAL_US), 
                               rightMotorPwm(PWM_NEUTRAL_US) {
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
    // Map inputs to valid PWM range (1000-2000us)
    leftMotorPwm = map(throttlePwm, PWM_MIN_US, PWM_MAX_US, PWM_MIN_US, PWM_MAX_US);
    rightMotorPwm = map(steeringPwm, PWM_MIN_US, PWM_MAX_US, PWM_MIN_US, PWM_MAX_US);
    
    // Write to servos
    leftMotorServo.writeMicroseconds(leftMotorPwm);
    rightMotorServo.writeMicroseconds(rightMotorPwm);
    
    Logger::getInstance()->log(LOG_DEBUG, "MOTOR", "Car mode PWM set");
}

// Set tank mode parameters
void MotorControl::setTankMode(int leftMotorPwm, int rightMotorPwm) {
    // Map inputs to valid PWM range (1000-2000us) and update state
    this->leftMotorPwm = map(leftMotorPwm, PWM_MIN_US, PWM_MAX_US, PWM_MIN_US, PWM_MAX_US);
    this->rightMotorPwm = map(rightMotorPwm, PWM_MIN_US, PWM_MAX_US, PWM_MIN_US, PWM_MAX_US);
    
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
