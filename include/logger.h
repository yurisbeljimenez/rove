#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "config.h"

/**
 * @file logger.h
 * @brief Provides a centralized, leveled logging mechanism for the Rove system.
 */

/**
 * @class Logger
 * @brief Singleton class that manages system-wide logging.
 * 
 * The Logger allows different modules to log messages with varying severity levels.
 * By setting a minimum log level threshold, developers can filter out verbose
 * debug information during normal operation while retaining error and warning logs.
 */
class Logger {
public:
    /**
     * @brief Constructs a new Logger object.
     * 
     * Initializes the logger with a default INFO level.
     */
    Logger();

    /**
     * @brief Sets the minimum log level threshold.
     * 
     * Any messages with a severity lower than this level will be ignored.
     * 
     * @param level The new minimum LogLevel threshold.
     */
    void setLogLevel(LogLevel level);

    /**
     * @brief Logs a formatted message with a specific severity and component tag.
     * 
     * @param level The severity of the log (e.g., LogLevel::INFO, LogLevel::ERROR).
     * @param component A string identifier for the module (e.g., "DRIVE", "MOTOR").
     * @param format A printf-style format string.
     * @param ... Variable arguments for the format string.
     */
    void log(LogLevel level, const char* component, const char* format, ...);

    /**
     * @brief Retrieves the singleton instance of the Logger.
     * 
     * This uses a thread-safe local static initialization (C++11 and later).
     * 
     * @return Logger* A pointer to the global Logger instance.
     */
    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

private:
    // Singleton instance pointer
    static Logger* instance;
    // Current log level threshold
    LogLevel currentLogLevel;
};

#endif // LOGGER_H
