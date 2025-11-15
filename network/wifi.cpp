#include "wifi.hpp"
#include "utils/log.hpp"

#define PINICORE_TAG_WIFI   "pcore_wifi"

void WiFiComm::init() {
    // Currently nothing to do
}

void WiFiComm::config(const char* ssid, const char* pass) {
    strncpy(m_ssid, ssid, sizeof(m_ssid));
    strncpy(m_pass, pass, sizeof(m_pass));
    LOG_D(PINICORE_TAG_WIFI, "Configured WiFi SSID: %s", m_ssid);
}

void WiFiComm::configAP(const char* ssid, const char* pass, bool hidden) {
    strncpy(m_ssidAP, ssid, sizeof(m_ssidAP));
    strncpy(m_passAP, pass, sizeof(m_passAP));
    m_isHiddenAP = hidden;
    LOG_D(PINICORE_TAG_WIFI, "Configured WiFi AP SSID: %s", m_ssidAP);
}

void WiFiComm::maintain() {
    // Currently nothing to do
}

bool WiFiComm::connect() {
    if (m_isActiveAP)
        WiFi.mode(WIFI_MODE_APSTA);
    else
        WiFi.mode(WIFI_MODE_STA);
    
    m_isActiveStation = true;
    wl_status_t status = WiFi.begin(m_ssid, m_pass);
    WiFi.setAutoReconnect(true);

    // WiFi.begin turns off the WiFi AP, so reenable it
    if (m_isActiveAP)
        connectAP();
    
    return (status == WL_CONNECTED);
}

void WiFiComm::disconnect() {
    m_isActiveStation = false;
    WiFi.disconnect(false, true);
}

bool WiFiComm::connectAP() {
    if (m_isActiveStation)
        WiFi.mode(WIFI_MODE_APSTA);
    else
        WiFi.mode(WIFI_MODE_AP);
    
    m_isActiveAP = true;
    return WiFi.softAP(m_ssidAP, m_passAP, 1, m_isHiddenAP);
}

void WiFiComm::disconnectAP() {
    m_isActiveAP = false;
    WiFi.softAPdisconnect(false);
}

uint8_t WiFiComm::stationsConnectedAP() {
    return WiFi.softAPgetStationNum();
}

void WiFiComm::enable() {
    WiFi.begin();
}

void WiFiComm::disable() {
    m_isActiveStation = false;
    m_isActiveAP = false;
    WiFi.disconnect(true);
}