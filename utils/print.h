#pragma once

#ifndef PINICORE_PRINT_H
#define PINICORE_PRINT_H

#include <stdio.h>
#include <HardwareSerial.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/**
 * @brief   Printf for boards that do not have Serial.printf implemented
 */
inline void serialPrintf(const char *format, ...) {
    char buf[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    Serial.print(buf);
}

#endif // PINICORE_PRINT_H