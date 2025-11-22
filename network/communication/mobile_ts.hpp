/**
* @file		mobile_ts.hpp
* @brief	Mobile variation for TS.
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

#ifndef _PINICORE_MOBILETS_H
#define _PINICORE_MOBILETS_H

#include "mobile.hpp"

/**
 * Currently only the SIM800 has been tested.
 * Other mobile modules might require adjustments to the code and refactoring.
 */
class MobileTSComm : public MobileComm {
    public:
        /**
         * @brief   Turn on the Mobile device.
         * @note    This version of 'enable' will auto detect the ESP board and will
         *          select the appropriate way to power on the module.
         * @warning Might show 'ERROR ... Unable to restart Mobile module! Possible problem with hardware.'
         *          in the serial while is iterating over the auto detection methods.
         *          This is considered normal while it has not completely iterated over all the 'EGsmVersion'.
         */
        void enable() override;

        /**
         * @brief   Turn off the Mobile device.
         */
        void disable() override;

    private:
        /**
         * @brief   Used to detect which ESP board the firmware is running on,
         *          to then select the appropriate way to 'enable'/'disable' the
         *          mobile module.
         */
        enum EGsmVersion { ESP_GSM, ESP_V2, SIZE_GSM_VERSION };

        /**
         * @brief   The detected ESP board.
         * @note    When 'p_connectedOnce' is 'true', this variable will be the detected ESP board.
         * @warning 'SIZE_GSM_VERSION' is not a valid version, set it to 'ESP_GSM' as a start.
         */
        EGsmVersion m_gsmVersion = EGsmVersion::SIZE_GSM_VERSION;
};

#endif /* _PINICORE_MOBILETS_H */
