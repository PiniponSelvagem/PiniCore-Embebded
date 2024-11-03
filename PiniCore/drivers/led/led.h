/**
* @file		led.h
* @brief	LED configuration API.
* @version	1.1
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

#ifndef PINICORE_LED_H
#define PINICORE_LED_H

#include "piniconst.h"

/**
 * @brief	Structure containing led information.
 */
struct LED_INFO {
    PIN pin;		    /** Board pin the led is connected to. */
};

/**
 * @brief	Maximum number of leds supported.
 */
#define LED_MAX  32

class Led {
    public:
        /**
         * @brief	LED constructor.
         * @param   leds: Led configuration array.
         * @param   nLeds: Number of leds in the array.
         * @note    If 'size' is above 'LED_MAX', only the first 'LED_MAX' leds will be configured with the rest being ignored.
         */
        Led(LED_INFO* leds, uint8_t nLeds);

        /**
         * @brief	Initializes the configured led.
         * @note	This function must be called prior to any other LED functions.
         */
        void init();

        /**
         * @brief	Checks for led state changes and their active state, and returns it as a mask.
         * @param	id: Led id, [0..LED_MAX-1] values outside this range will be declared as invalid led.
         * @return	'true' when led is ON and 'false' when led is OFF.
         * @warning Since this function uses an internal state, if the led state is changed without using this API, it might return incorrect results.
         */
        bool getState(uint8_t id);

        /**
         * @brief	Turns the led ON or OFF depend on the state.
         * @param	id: Led id, [0..LED_MAX-1] values outside this range will be declared as invalid led.
         * @param   state: State to change the led into.
         */
        void set(uint8_t id, bool state);

        /**
         * @brief	Toggles led state ON/OFF based on last state.
         * @param	id: Led id, [0..LED_MAX-1] values outside this range will be declared as invalid led.
         * @warning Since this function uses an internal state, if the led state is changed without using this API, it might toggle to an unexpected state.
         */
        void toggle(uint8_t id);

    private:
        /**
         * @brief	Array of 'LED_INFO' structures containing all leds to be configured when \ref 'init' is called.
         * @note	Led definition, see \ref 'LED_INFO' on how a led is defined.
         */
        struct LED_INFO m_leds[LED_MAX];

        /**
         * @brief   Number of configured leds.
         */
        uint8_t m_nLeds = 0;

        /**
         * @brief	Current state of all leds.
         * 			Each led is represented with 1 bit.
         * 			1st bit -> 1st led active state (0 -> off, 1 -> on)
         * 			2nd bit -> 2nd led active state
         * 			...
         * 			31th bit -> 31th led active state
         * 			32th bit -> 32th led active state
         */
        uint32_t m_ledsCurrentState = 0;
};


#endif /* PINICORE_LED_H */
