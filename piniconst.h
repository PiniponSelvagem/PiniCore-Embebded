/**
* @file		pinicore.h
* @brief	Definitions and constants used by PiniCore library.
* @version	1.0
* @date		12 Jun 2024
* @author	PiniponSelvagem
*
* Copyright(C) PiniponSelvagem
*
***********************************************************************
* Software that is described here, is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
**********************************************************************/

#ifndef PINICORE_CONST_H
#define PINICORE_CONST_H

#include <stdint.h>

#ifdef ARDUINO
    #define OFF     0
    #define ON      1

    #ifdef ARDUINO_AVR_MEGA2560
        #define LED_BUILTIN     13
    #endif
#endif

typedef uint8_t  PIN;


#endif /* PINICORE_CONST_H */
