#include "logger.h"

// Static instance pointer initialization
Logger* Logger::instance = nullptr;

/**
 * @brief Constructs a new Logger object.
 * 
 * Initializes the logger with a default INFO level.
 */
Logger::Logger() : currentLogLevel(LogLevel::INFO) {
    // Constructor implementation
}

/**
 * @brief Sets the minimum log level threshold.
 * 
 * Any messages with a severity lower than this level will be ignored.
 * 
 * @param level The new minimum LogLevel threshold.
 */
void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

/**
 * @brief Logs a formatted message with a specific severity and component tag.
 * 
 * @param level The severity of the log (e.g., LogLevel::INFO, LogLevel::ERROR).
 * @param component A string identifier for the module (e.g., "DRIVE", "MOTOR").
 * @param format A printf-style format string.
 * @param ... Variable arguments for the format string.
 */
void Logger::log(LogLevel level, const char* component, const char* format, ...) {
    // Check if this log level should be displayed
    if (level < currentLogLevel) {
        return;
    }
    
    // Format the log message
    va_list args;
    va_start(args, format);
    
    // Determine prefix based on log level
    const char* levelPrefix;
    switch (level) {
        case LogLevel::DEBUG:
            levelPrefix = "[DEBUG]";
            break;
        case LogLevel::INFO:
            levelPrefix = "[INFO]";
            break;
        case LogLevel::WARN:
            levelPrefix = "[WARN]";
            break;
        case LogLevel::ERROR:
            levelPrefix = "[ERROR]";
            break;
        default:
            levelPrefix = "[LOG]";
            break;
    }
    
    // Print the formatted message
    Serial.print(levelPrefix);
    Serial.print(" [");
    Serial.print(component);
    Serial.print("] ");
    vprintf(format, args);
    Serial.println();
    
    va_end(args);
}