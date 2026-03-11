#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define PIN_ESC_LEFT        1
#define PIN_ESC_RIGHT       2

// PWM constants
#define PWM_NEUTRAL_US      1500
#define PWM_MIN_US          1000
#define PWM_MAX_US          2000

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

#endif // CONFIG_H