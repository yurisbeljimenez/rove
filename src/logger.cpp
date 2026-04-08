#include "logger.h"

// Static instance pointer initialization
Logger* Logger::instance = nullptr;

// Constructor
Logger::Logger() : currentLogLevel(LOG_INFO) {
    // Constructor implementation
}

// Set the minimum log level threshold
void Logger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

// Log a message with specified level and component ID
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
        case LOG_DEBUG:
            levelPrefix = "[DEBUG]";
            break;
        case LOG_INFO:
            levelPrefix = "[INFO]";
            break;
        case LOG_WARN:
            levelPrefix = "[WARN]";
            break;
        case LOG_ERROR:
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