#pragma once

#ifndef PINICORE_LOG_H
#define PINICORE_LOG_H

#include "utils/time.hpp"

#ifdef ARDUINO_AVR_MEGA2560
    #include "print.h"
    #define PRINTF  serialPrintf
#else
    #define PRINTF  Serial.printf
#endif


/**
 * Example of PLOG_LEVEL setting:
 * #define PLOG_LEVEL PLOG_LEVEL_DEBUG
 * 
 * This should be defined in before including this LOG header file.
 */

/**
 * Want colored log messages?
 * Use this define before including this LOG header file:
 * #define PLOG_COLORED
 */


// Define log levels
#define PLOG_LEVEL_NONE     0
#define PLOG_LEVEL_FATAL    1
#define PLOG_LEVEL_ERROR    2
#define PLOG_LEVEL_WARN     3
#define PLOG_LEVEL_INFO     4
#define PLOG_LEVEL_DEBUG    5
#define PLOG_LEVEL_TRACE    6

#ifdef PLOG_COLORED
    const char PLOG_TEXT_FATAL[]   = "\e[1;41mFATAL\e[0m";
    const char PLOG_TEXT_ERROR[]   = "\e[1;31mERROR\e[0m";
    const char PLOG_TEXT_WARN[]    = "\e[1;33mWARN\e[0m ";
    const char PLOG_TEXT_INFO[]    = "\e[1;32mINFO\e[0m ";
    const char PLOG_TEXT_DEBUG[]   = "\e[1;37mDEBUG\e[0m";
    const char PLOG_TEXT_TRACE[]   = "\e[1;36mTRACE\e[0m";
#else
    const char PLOG_TEXT_FATAL[]   = "FATAL"
    const char PLOG_TEXT_ERROR[]   = "ERROR"
    const char PLOG_TEXT_WARN[]    = "WARN "
    const char PLOG_TEXT_INFO[]    = "INFO "
    const char PLOG_TEXT_DEBUG[]   = "DEBUG"
    const char PLOG_TEXT_TRACE[]   = "TRACE"
#endif


// Helper macro to generate the log
#define PLOG_IMPL(logLevelStr, klass, fmt, ...) \
    PRINTF("%s (%llu) [%s] " fmt "\n", logLevelStr, getMillis(), klass, ##__VA_ARGS__)


// Level-specific macros
#if PLOG_LEVEL >= PLOG_LEVEL_FATAL
  #define LOG_F(klass, fmt, ...) PLOG_IMPL(PLOG_TEXT_FATAL, klass, fmt, ##__VA_ARGS__)
#else
  #define LOG_F(klass, fmt, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_ERROR
  #define LOG_E(klass, fmt, ...) PLOG_IMPL(PLOG_TEXT_ERROR, klass, fmt, ##__VA_ARGS__)
#else
  #define LOG_E(klass, fmt, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_WARN
  #define LOG_W(klass, fmt, ...) PLOG_IMPL(PLOG_TEXT_WARN, klass, fmt, ##__VA_ARGS__)
#else
  #define LOG_W(klass, fmt, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_INFO
  #define LOG_I(klass, fmt, ...) PLOG_IMPL(PLOG_TEXT_INFO, klass, fmt, ##__VA_ARGS__)
#else
  #define LOG_I(klass, fmt, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_DEBUG
  #define LOG_D(klass, fmt, ...) PLOG_IMPL(PLOG_TEXT_DEBUG, klass, fmt, ##__VA_ARGS__)
#else
  #define LOG_D(klass, fmt, ...)
#endif

#if PLOG_LEVEL >= PLOG_LEVEL_TRACE
  #define LOG_T(klass, fmt, ...) PLOG_IMPL(PLOG_TEXT_TRACE, klass, fmt, ##__VA_ARGS__)
#else
  #define LOG_T(klass, fmt, ...)
#endif



#endif // PINICORE_LOG_H