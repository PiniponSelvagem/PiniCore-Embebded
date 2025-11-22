#include "mobile_ts.hpp"

void MobileTSComm::enable() {
    digitalWrite(p_pinPowerKey, LOW);
    digitalWrite(p_pinReset, HIGH);
    digitalWrite(p_pinPowerOn, LOW);
    delay(100);
    digitalWrite(p_pinPowerOn, HIGH);

    if (!p_connectedOnce) {
        /**
         * No longer enter here if connected at least once, meaning the module was correctly
         * detected and further 'disable'/'enable' do not require redetection.
         */
        switch (m_gsmVersion) {
            case EGsmVersion::ESP_GSM: { m_gsmVersion = EGsmVersion::ESP_V2;  break; }
            default:                   { m_gsmVersion = EGsmVersion::ESP_GSM; break; }
        }
    }

    switch (m_gsmVersion) {
        case EGsmVersion::ESP_V2:  { digitalWrite(p_pinPowerKey, HIGH); break; }
        default:
            break;
    }
}

void MobileTSComm::disable() {
    switch (m_gsmVersion) {
        case EGsmVersion::ESP_V2:  { digitalWrite(p_pinPowerKey, LOW);  break; }
        default:                   { digitalWrite(p_pinPowerKey, HIGH); break; }
    }
    digitalWrite(p_pinReset, LOW);
    digitalWrite(p_pinPowerOn, HIGH);
}
