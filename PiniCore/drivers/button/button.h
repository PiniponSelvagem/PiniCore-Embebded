/**
* @file		button.h
* @brief	Button API.
* @version	1.1
* @date		13 Jun 2024
* @author	PiniponSelvagem
*
* Copyright(C) PiniponSelvagem
*
***********************************************************************
* Software that is described here, is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
**********************************************************************/

#ifndef PINICORE_BUTTON_H
#define PINICORE_BUTTON_H

#include "piniconst.h"

/**
 * @brief	Structure containing button information.
 */
struct BTN_INFO {
    PIN pin;		    /** Board pin the button is connected to. */
};

/**
 * @brief	Maximum number of buttons supported.
 */
#define BUTTON_MAX  16

class Button {
    public:
        /**
         * @brief	Button constructor.
         * @param	btns: Button configuration array.
         * @param	nBtns: Number of buttons in the array.
         * @note    If 'size' is above 'BUTTON_MAX', only the first 'BUTTON_MAX' buttons will be configured with the rest being ignored.
         */
        Button(BTN_INFO* btns, uint8_t nBtns);

        /**
         * @brief	Initializes the configured buttons.
         * @note	This function must be called prior to any other Button functions.
         */
        void init();

        /**
         * @brief	Checks for button state changes and their active state, and returns it as a mask.
         * @note	This function updates all buttons last state, meaning that calling this multiple times you might lose their state if you not checking them.
         * 			Clears simulated buttons if they were active.
         */
        void pullEvents();

        /**
         * @brief	Get buttons raw snapshot.
         * @return	Last button snapshot created from last call of \ref 'pullEvents'.
         */
        uint32_t getSnapshot();

        /**
         * @brief	Get button pressed status.
         * @param	id: Button id, [0..BUTTON_MAX-1] values outside this range will be declared as invalid button.
         * @return	Current state for this button. On invalid, returns 'false'.
         * @note	This information is based on last call of \ref 'updateButtonsEvents'.
         */
        bool isPressed(uint8_t id);

        /**
         * @brief	Get button changed status. Goes 1 on first press and then right after 0, goes 1 again of release and then right after 0.
         * @param	id: Button id, [0..BUTTON_MAX-1] values outside this range will be declared as invalid button.
         * @return	If button changed since last pull of events. On invalid, returns 'false'.
         * @note	This information is based on last call of \ref 'updateButtonsEvents'.
         */
        bool isChanged(uint8_t id);

        /**
         * @brief	Get button repeating status and if its pressed.
         * @param	id: Button id, [0..BUTTON_MAX-1] values outside this range will be declared as invalid button.
         * @return	If button is currently being pressed and is repeating. On invalid, returns 'false'.
         * @note	This information is based on last call of \ref 'updateButtonsEvents'.
         */
        bool isPressedRepeating(uint8_t id);

        /**
         * @brief	Get button single press status. "On press."
         * @param	id: Button id, [0..BUTTON_MAX-1] values outside this range will be declared as invalid button.
         * @return	Get button press status as single press, returning 'false' afterwards. On invalid, returns 'false'.
         * @note	This information is based on last call of \ref 'updateButtonsEvents'.
         */
        bool isSinglePressed(uint8_t id);

        /**
         * @brief	Get button single release status. "On release."
         * @param	id: Button id, [0..BUTTON_MAX-1] values outside this range will be declared as invalid button.
         * @return	Get button release status as single release, returning 'false' afterwards. On invalid, returns 'false'.
         * @note	This information is based on last call of \ref 'updateButtonsEvents'.
         */
        bool isSingleReleased(uint8_t id);

        /**
         * @brief	Return 'true' on any key press.
         * @return	'true' on any key press.
         */
        bool isAnyKeyPressed();

    private:
        /**
         * @brief	Array of 'BTN_INFO' structures containing all buttons to be configured when \ref 'init' is called.
         * @note	Button definition, see \ref 'BTN_INFO' on how a button is defined.
         */
        struct BTN_INFO m_btns[BUTTON_MAX];

        /**
         * @brief   Number of configured buttons.
         */
        uint8_t m_nButtons = 0;

        /**
         * @brief	Last state of all buttons, including their transition state.
         * 			Mask with button last state and their transition state.
         * 			Each button is represented with 2 bits.
         * 			1st bit -> 1st button active state (0 -> inactive, 1 -> active)
         * 			2nd bit -> 1st button transition state (0 -> is repeating last state, 1 -> button transitioned to new state)
         * 			...
         * 			31th bit -> 16th button active state
         * 			32th bit -> 16th button transition state
         */
        uint32_t m_btnsLastState = 0;
};

#endif /* PINICORE_BUTTON_H */
