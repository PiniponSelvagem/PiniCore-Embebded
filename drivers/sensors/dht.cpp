#include "dht.hpp"
#include <Arduino.h>
#include "utils/time.hpp"
#include "utils/log.hpp"

void DigitalHT::init(uint8_t pin, EDHT type) {
    m_dht = new DHT(pin, (uint8_t)type);
    m_dht->begin();
}

float DigitalHT::readTemperature() {
    return m_dht->readTemperature();
}

float DigitalHT::readHumidity() {
    return m_dht->readHumidity();
}
