#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "config.h"

/**
 * @brief Centralized Logger class for Rove system
 * 
 * This class provides a centralized logging mechanism that can be configured
 * with different log levels to filter messages. Each module passes its ID 
 * when logging to identify the source.
 */
class Logger {
public:
    /**
     * @brief Construct a new Logger object
     */
    Logger();

    /**
     * @brief Set the minimum log level threshold
     * 
     * @param level Minimum level to display (DEBUG, INFO, WARN, ERROR, OFF)
     */
    void setLogLevel(LogLevel level);

    /**
     * @brief Log a message with specified level and component ID
     * 
     * @param level Log level of the message
     * @param component Component ID string (e.g., "DRIVE", "INPUT")
     * @param format Format string for the message
     * @param ... Variable arguments for formatting
     */
    void log(LogLevel level, const char* component, const char* format, ...);

    /**
     * @brief Get singleton instance
     * 
     * @return Logger* Pointer to singleton instance
     */
    static Logger* getInstance() {
        static Logger instance;
        return &instance;
    }

private:
    // Current log level threshold
    LogLevel currentLogLevel;
};

#endif // LOGGER_H
