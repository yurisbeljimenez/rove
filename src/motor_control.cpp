#include "motor_control.h"
#include "config.h"
#include "logger.h"
#include <ESP32Servo.h>

// Global servo objects for motor control
Servo leftMotorServo;
Servo rightMotorServo;

MotorControl::MotorControl() : leftMotorPwm(PWM_NEUTRAL_US), 
                               rightMotorPwm(PWM_NEUTRAL_US) {}

void MotorControl::begin() {
    leftMotorServo.attach(PIN_ESC_LEFT, PWM_MIN_US, PWM_MAX_US);
    rightMotorServo.attach(PIN_ESC_RIGHT, PWM_MIN_US, PWM_MAX_US);
    setNeutral();
    
    Logger::getInstance()->log(LOG_INFO, "MOTOR", "Motor control initialized");
}

void MotorControl::setCarMode(int throttlePwm, int steeringPwm) {
    leftMotorPwm = map(throttlePwm, PWM_MIN_US, PWM_MAX_US, PWM_MIN_US, PWM_MAX_US);
    rightMotorPwm = map(steeringPwm, PWM_MIN_US, PWM_MAX_US, PWM_MIN_US, PWM_MAX_US);
    
    leftMotorServo.writeMicroseconds(leftMotorPwm);
    rightMotorServo.writeMicroseconds(rightMotorPwm);
}

void MotorControl::setTankMode(int leftMotorPwm, int rightMotorPwm) {
    this->leftMotorPwm = map(leftMotorPwm, PWM_MIN_US, PWM_MAX_US, PWM_MIN_US, PWM_MAX_US);
    this->rightMotorPwm = map(rightMotorPwm, PWM_MIN_US, PWM_MAX_US, PWM_MIN_US, PWM_MAX_US);
    
    leftMotorServo.writeMicroseconds(this->leftMotorPwm);
    rightMotorServo.writeMicroseconds(this->rightMotorPwm);
}

void MotorControl::setNeutral() {
    leftMotorPwm = PWM_NEUTRAL_US;
    rightMotorPwm = PWM_NEUTRAL_US;
    
    leftMotorServo.writeMicroseconds(leftMotorPwm);
    rightMotorServo.writeMicroseconds(rightMotorPwm);
}
