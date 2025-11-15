/**
* @file		mobile.hpp
* @brief	Mobile network API.
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

#ifndef _PINICORE_MOBILE_H
#define _PINICORE_MOBILE_H

#include "inetwork.hpp"
#include "piniconst.hpp"

#ifndef _PINICORE_CONFIG_H
    #include "piniconfig.hpp"
#endif

#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
//#define TINY_GSM_DEBUG Serial

#include <TinyGsmClient.h>

#define PINICORE_MOBILE_SSID_SIZE_MAX 64
#define PINICORE_MOBILE_IMSI_SIZE_MAX 64

class MobileComm : public INetwork {
    public:
        /**
         * @brief	Initializes the network interface.
         * @note	This function must be called prior to any other MobileComm functions.
         */
        void init() override;

        /**
         * @brief   Configure Mobile connection or replace the current configuration with a new one.
         * @param   pinPowerOn Modem power on pin.
         * @param   pinPowerKey Modem power key pin.
         * @param   pinPowerReset Modem reset pin.
         * @param   pinPowerTx Modem Tx pin.
         * @param   pinPowerRx Modem Rx pin.
         * @param   ssid Cell APN identifier.
         * @param   user Cell network user.
         * @param   pass Cell network password.
         * @param   pin SIM card unlocking pin.
         * @note    If already connected, requires \ref 'disconnect' and then \ref 'connect' for new configuration to take place.
         */
        void config(
            uint8_t pinPowerOn, uint8_t pinPowerKey, uint8_t pinReset, uint8_t pinTX, uint8_t pinRx,
            const char* ssid, const char* user, const char* pass, const char* pin
        );

        /**
         * @brief   Keeps the Mobile connection alive, reconnects if disconnected.
         * @note    Call this function periodically to maintain the network alive.
         */
        void maintain() override;

        /**
         * @brief   Connect to the assigned Mobile network.
         * @return  'True' if able to connect.
         */
        bool connect() override;

        /**
         * @brief   Disconnect from the current Mobile network.
         */
        void disconnect() override;

        /**
         * @brief   Turn on the Mobile device.
         */
        virtual void enable() override;

        /**
         * @brief   Turn off the Mobile device.
         */
        virtual void disable() override;

        /**
         * @brief   Get the type of this network interface.
         * @return  Enum value of the network type.
         */
        inline ENetworkType getType() const override { return ENetworkType::NET_MOBILE; };

        /**
         * @brief   Get the Client network object.
         * @return  Pointer to Client object.
         */
        inline const Client* getClient() const override { return m_client; };

        /**
         * @brief   Get the name of the current connected network.
         * @return  Pointer to a null terminated char array with the name of the currently connected network.
         */
        inline const char* getName() const override { return m_providerName; }

        /**
         * @brief   Get the IMSI of the SIM card.
         * @return  Pointer to a null terminated char array with the IMSI.
         */
        inline const char* getIMSI() const { return m_providerName; }

        /**
         * @brief   Get signal strength of the current connected network.
         * @return  Value in dB.
         */
        inline int getSignalStrength() const override { return modem.getSignalQuality(); }

        /**
         * @brief   Get connection state.
         * @return  'true' if connected 'false' otherwise.
         */
        inline bool isConnected() const override { return false; };


    private:
        char m_providerName[PINICORE_MOBILE_SSID_SIZE_MAX];
        char m_imsi[PINICORE_MOBILE_IMSI_SIZE_MAX];
        TinyGsmClient *m_client = &gsmClient;

        mutable TinyGsm modem = TinyGsm(SerialAT);
        TinyGsmClient gsmClient = TinyGsmClient(modem, 0);
};

#endif /* _PINICORE_MOBILE_H */
