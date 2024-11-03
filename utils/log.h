#pragma once

#ifndef PINICORE_LOG_H
#define PINICORE_LOG_H

#ifdef ARDUINO_AVR_MEGA2560
    #include "print.h"
    #define PRINTF  serialPrintf
#else
    #define PRINTF  Serial.printf
#endif 

/**
 * Example of LOG_LEVEL setting:
 * #define LOG_LEVEL LOG_LEVEL_DEBUG
 * 
 * This should be defined in GLOBAL and before including this LOG header file.
 */

/**
 * Want colored log messages?
 * Use this define in GLOBAL before including this LOG header file:
 * #define LOG_COLORED
 */


// Define log levels
#define LOG_LEVEL_NONE    0
#define LOG_LEVEL_FATAL   1
#define LOG_LEVEL_ERROR   2
#define LOG_LEVEL_WARN    3
#define LOG_LEVEL_INFO    4
#define LOG_LEVEL_DEBUG   5
#define LOG_LEVEL_TRACE   6

#ifdef LOG_COLORED
#define LOG_TEXT_FATAL  "\e[1;41mFATAL: \e[0m"
#define LOG_TEXT_ERROR  "\e[1;31mERROR: \e[0m"
#define LOG_TEXT_WARN   "\e[1;33mWARN:  \e[0m"
#define LOG_TEXT_INFO   "\e[1;32mINFO:  \e[0m"
#define LOG_TEXT_DEBUG  "\e[1;37mDEBUG: \e[0m"
#define LOG_TEXT_TRACE  "\e[1;36mTRACE: \e[0m"
#else
#define LOG_TEXT_FATAL  "FATAL: "
#define LOG_TEXT_ERROR  "ERROR: "
#define LOG_TEXT_WARN   "WARN:  "
#define LOG_TEXT_INFO   "INFO:  "
#define LOG_TEXT_DEBUG  "DEBUG: "
#define LOG_TEXT_TRACE  "TRACE: "
#endif


// Define logging macros based on compile-time selection
#if LOG_LEVEL >= LOG_LEVEL_FATAL
  #define FATAL(klass, ...)    PRINTF(LOG_TEXT_FATAL "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define FATAL(klass, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
  #define ERROR(klass, ...)    PRINTF(LOG_TEXT_ERROR "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define ERROR(klass, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
  #define WARN(klass, ...)     PRINTF(LOG_TEXT_WARN "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define WARN(klass, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
  #define INFO(klass, ...)     PRINTF(LOG_TEXT_INFO "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define INFO(klass, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
  #define DEBUG(klass, ...)    PRINTF(LOG_TEXT_DEBUG "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define DEBUG(klass, ...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_TRACE
  #define TRACE(klass, ...)    PRINTF(LOG_TEXT_TRACE "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define TRACE(klass, ...)
#endif



/* // This is a nice option, but its checked every time during runtime, not good
// Generic log macro
#define LOG(level, prefix, ...) \
    do { if (level <= LOG_LEVEL) Serial.printf(prefix __VA_ARGS__); Serial.println(); } while (0)

// Define specific logging macros
#define FATAL(...) LOG(LOG_LEVEL_FATAL, "FATAL: ", __VA_ARGS__)
#define ERROR(...) LOG(LOG_LEVEL_ERROR, "ERROR: ", __VA_ARGS__)
#define WARN(...)  LOG(LOG_LEVEL_WARN,  "WARN:  ", __VA_ARGS__)
#define INFO(...)  LOG(LOG_LEVEL_INFO,  "INFO:  ", __VA_ARGS__)
#define DEBUG(...) LOG(LOG_LEVEL_DEBUG, "DEBUG: ", __VA_ARGS__)
#define TRACE(...) LOG(LOG_LEVEL_TRACE, "TRACE: ", __VA_ARGS__)
*/

#endif // PINICORE_LOG_H