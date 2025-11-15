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

#ifndef _PINICORE_H
#define _PINICORE_H

#include "utils/log.hpp"
#include "utils/time.hpp"
#include "utils/watchdog.hpp"

#include "system/board.hpp"

#include "drivers/button/button.hpp"
#include "drivers/led/led.hpp"
#include "drivers/rotaryencoder/rotaryencoder.hpp"

#include "network/wifi.hpp"
#include "network/mobile.hpp"

#endif /* _PINICORE_H */
