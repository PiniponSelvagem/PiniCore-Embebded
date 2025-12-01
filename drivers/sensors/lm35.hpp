/**
* @file		lm35.hpp
* @brief	Driver for temperature sensor LM35.
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

#ifndef _PINICORE_SENSOR_LM35_H
#define _PINICORE_SENSOR_LM35_H

#include <stdint.h>

class LM35 {
    public:
        virtual ~LM35() = default;

        /**
         * @brief	Initializes the sensor.
         * @note	This function must be called prior to any other sensor specific functions.
         */
        void init(uint8_t pin);

        /**
         * @brief   Read current temperature from hardware.
         * @return  Temperature in ºC.
         */
        float readTemperature();

    private:
        uint8_t m_pin;
};

#endif /* _PINICORE_TEMPERATURE_LM35_H */
