/**
* @file		dht11.hpp
* @brief	Driver for temperature and humidity sensor DHT.
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

#ifndef _PINICORE_SENSOR_DHT_H
#define _PINICORE_SENSOR_DHT_H

#include <stdint.h>
#include <DHT.h>

enum EDHT {
    DHT_11=11,
    DHT_12=12,
    DHT_21=21,
    DHT_22=22
};

class DigitalHT {
    public:
        virtual ~DigitalHT() = default;

        /**
         * @brief	Initializes the sensor.
         * @param   pin Pin the sensor is connected to.
         * @param   type DHT sensor variant type.
         * @note	This function must be called prior to any other sensor specific functions.
         */
        void init(uint8_t pin, EDHT type);

        /**
         * @brief   Read current temperature from hardware.
         * @return  Temperature in ºC.
         */
        float readTemperature();

        /**
         * @brief   Read current humidity from hardware.
         * @return  Humidity in %.
         */
        float readHumidity();

    private:
        /**
         * @brief   Read data from hardware, cache it and avoid unnecessary over reading the hardware.
         */
        void read();

        
        DHT* m_dht;

        int m_temperature;
        int m_humidity;
};

#endif /* _PINICORE_SENSOR_DHT_H */
