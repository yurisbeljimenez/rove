#include <Arduino.h>
#include "config.h"
#include "input_handler.h"
#include "drive_state.h"
#include "motor_control.h"

// Global instances
InputHandler inputHandler;
DriveState driveState;
MotorControl motorControl;

void setup() {
    Serial.begin(115200);
    
    motorControl.begin();
    inputHandler.begin();
    driveState.begin();
    
    inputHandler.setDriveState(&driveState);
}

void loop() {
    driveState.update(&inputHandler);
    delay(10);
}
