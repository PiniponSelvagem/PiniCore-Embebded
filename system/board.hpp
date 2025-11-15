/**
* @file		board.hpp
* @brief	Get information tied to the current board.
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

#ifndef _PINICORE_BOARD_H
#define _PINICORE_BOARD_H

/**
 * @brief Get device unique identifier, that is equal to the WiFi module MAC ADDRESS.
 * @return Char array null terminated with the device unique identifier string.
 */
const char* getUniqueId();

/**
 * @brief Asks the SOC what was the last reset reason, if was manual, return true.
 * @return True when was a manual reset.
 */
bool wasLastResetManual();

/**
 * @brief Asks the SOC if the last reset was fatal due to bad coding skills.
 * @return If you wrote bad code and this returned TRUE, you know what to do ;)
 */
bool wasLastResetFatal();

#endif // _PINICORE_BOARD_H