/**
* @file		pinicore.h
* @brief	Easy include PiniCore library.
* @author	PiniponSelvagem
*
* Copyright(C) PiniponSelvagem
*
***********************************************************************
* Software that is described here, is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
**********************************************************************/

#pragma once

/**
 * TODO:
 * - Add support for Arduino boards, currently only compiles on ESP-IDF based ones.
 */

/**
  * Dependencies:
  * lib_deps = 
  *     vshymanskyy/TinyGSM@^0.12
  *     https://github.com/alkonosst/SSLClientESP32.git#v2.0.3
  *     arduino-libraries/ArduinoHttpClient@^0.6.1
  */
#ifndef _PINICORE_H
#define _PINICORE_H

#include "utils/log.hpp"
#include "utils/time.hpp"
#include "utils/watchdog.hpp"
#include "utils/memory.hpp"

#include "system/board.hpp"

#include "drivers/button/button.hpp"
#include "drivers/led/led.hpp"
#include "drivers/rotaryencoder/rotaryencoder.hpp"

#include "network/communication/inetwork.hpp"
#include "network/communication/wifi.hpp"
#include "network/communication/mobile.hpp"

#include "network/request/ota/iota.hpp"
#include "network/request/ota/ota_ts.hpp"

#include "network/request/mqtt/mqtt.hpp"

#endif /* _PINICORE_H */
