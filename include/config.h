#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief Global configuration constants and types for the Rove firmware.
 */

// Pin definitions
/** @brief PWM output pin for the left motor/ESC */
#define PIN_ESC_LEFT        1
/** @brief PWM output pin for the right motor/steering servo */
#define PIN_ESC_RIGHT       2

// PWM constants
/** @brief Neutral PWM value in microseconds (standard for most ESCs) */
#define PWM_NEUTRAL_US      1500
/** @brief Minimum PWM value in microseconds */
#define PWM_MIN_US          1000
/** @brief Maximum PWM value in microseconds */
#define PWM_MAX_US          2000

// Deadzone threshold for analog sticks (0.1 = 10% deadzone)
/** @brief Threshold to ignore small stick movements to prevent drift */
#define AXIS_DEADZONE_THRESHOLD 0.1f

/**
 * @brief Logging severity levels.
 */
enum class LogLevel {
    /** Detailed information for debugging purposes */
    DEBUG,
    /** General operational messages */
    INFO,
    /** Warning messages indicating potential issues */
    WARN,
    /** Error messages indicating critical failures */
    ERROR
};

#endif // CONFIG_H
