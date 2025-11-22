/**
* @file		ota_ts.hpp
* @brief	OTA using the TS service.
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

#ifndef _PINICORE_OTATS_H
#define _PINICORE_OTATS_H

#include "iota.hpp"
#include <stdint.h>

#define OTA_TS_AVAILABLEUPDATE_URI_MAX_SIZE 128

class OTATS : public IOTA{
    public:
        /**
         * @brief	OTA constructor.
         * @param	client Pointer to network client to be used for requests.
         * @param   currFirmware Current firmware version.
         * @param   serial Controller serial / uniqueID, optional for APIs that do not require it.
         */
        OTATS(Client *client, int currFirmware, const char* serial = NULL)
        : IOTA(client, currFirmware, serial) { }

        /**
         * @brief   Adds and enables the usage of a SSL certificate.
         * @param   certificate SSL certificate, char* starting from '-----BEGIN CERTIFICATE-----\n' until '-----END CERTIFICATE-----\n' inclusive.
         */
        void addCertificate(const char* certificate);

        /**
         * @brief   Check for updates based on the current firmware version.
         * @return  Available update status.
         */
        bool checkUpdate() override;

        /**
         * @brief   Download and install the available update.
         * @return  Update status code, if 'OTA_INSTALLED' then next reboot will execute new update.
         * @warning \ref 'checkUpdate' must be called before calling this function, otherwise it
         *          will return 'OTA_UPDATE_NOT_AVAILABLE' regardless if an update exists or not.
         */
        EOTAUpdateStatus update() override;


    private:
        const char* m_certificate;

        char m_uriAvailableUpdate[OTA_TS_AVAILABLEUPDATE_URI_MAX_SIZE];
};

#endif // _PINICORE_OTATS_H