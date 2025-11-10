#pragma once

#ifndef PINICORE_TIME_H
#define PINICORE_TIME_H

#include <esp_timer.h>

inline uint64_t getMillis() {
    return static_cast<uint64_t>(esp_timer_get_time()) / 1000LL;
}


#endif // PINICORE_TIME_H