#include "iota.hpp"

IOTA::IOTA(Client *client, int currFirmware, const char* serial) {
    m_client       = client;
    m_currFirmware = currFirmware;
    m_serial       = serial;
}

void IOTA::setProgressCallback(OTA_ONPROGRESS_SIGNATURE onProgress) {
    m_onProgress = onProgress;
}

void IOTA::onProgress(uint32_t downloadedBytes, uint32_t totalBytes) {
    if (m_onProgress != NULL)
        m_onProgress(downloadedBytes, totalBytes);
}
