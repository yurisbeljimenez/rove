#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define PIN_ESC_LEFT        1
#define PIN_ESC_RIGHT       2
#define PIN_CONTROLLER_LED  3

// PWM constants
#define PWM_NEUTRAL_US      1500
#define PWM_MIN_US          1000
#define PWM_MAX_US          2000

// Controller button mappings
#define BUTTON_Y            16  // Y button on Xbox controller
#define BUTTON_B            18  // B button on Xbox controller

// Log levels
typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

#endif // CONFIG_H
