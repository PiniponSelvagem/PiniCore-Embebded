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
#define PLOG_LEVEL_NONE    0
#define PLOG_LEVEL_FATAL   1
#define PLOG_LEVEL_ERROR   2
#define PLOG_LEVEL_WARN    3
#define PLOG_LEVEL_INFO    4
#define PLOG_LEVEL_DEBUG   5
#define PLOG_LEVEL_TRACE   6

#ifdef PLOG_COLORED
#define PLOG_TEXT_FATAL  "\e[1;41mFATAL: \e[0m"
#define PLOG_TEXT_ERROR  "\e[1;31mERROR: \e[0m"
#define PLOG_TEXT_WARN   "\e[1;33mWARN:  \e[0m"
#define PLOG_TEXT_INFO   "\e[1;32mINFO:  \e[0m"
#define PLOG_TEXT_DEBUG  "\e[1;37mDEBUG: \e[0m"
#define PLOG_TEXT_TRACE  "\e[1;36mTRACE: \e[0m"
#else
#define PLOG_TEXT_FATAL  "FATAL: "
#define PLOG_TEXT_ERROR  "ERROR: "
#define PLOG_TEXT_WARN   "WARN:  "
#define PLOG_TEXT_INFO   "INFO:  "
#define PLOG_TEXT_DEBUG  "DEBUG: "
#define PLOG_TEXT_TRACE  "TRACE: "
#endif


// Define logging macros based on compile-time selection
#if PLOG_LEVEL >= PLOG_LEVEL_FATAL
  #define FATAL(klass, ...)    PRINTF(PLOG_TEXT_FATAL "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define FATAL(klass, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_ERROR
  #define ERROR(klass, ...)    PRINTF(PLOG_TEXT_ERROR "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define ERROR(klass, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_WARN
  #define WARN(klass, ...)     PRINTF(PLOG_TEXT_WARN "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define WARN(klass, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_INFO
  #define INFO(klass, ...)     PRINTF(PLOG_TEXT_INFO "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define INFO(klass, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_DEBUG
  #define DEBUG(klass, ...)    PRINTF(PLOG_TEXT_DEBUG "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define DEBUG(klass, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_TRACE
  #define TRACE(klass, ...)    PRINTF(PLOG_TEXT_TRACE "[" klass "] " __VA_ARGS__ ); Serial.println();
#else
  #define TRACE(klass, ...)
#endif


#endif // PINICORE_LOG_H