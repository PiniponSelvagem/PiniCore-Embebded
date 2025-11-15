#include "led.hpp"
#include <Arduino.h>

/**
 * @brief	Led state mask.
 */
#define LED_STATE_MASK	0x1

#define LED_GET_STATE(id)           (m_ledsCurrentState & (LED_STATE_MASK << id))
#define LED_SET_STATE(id, state)    m_ledsCurrentState = ((m_ledsCurrentState & ~(LED_STATE_MASK << id)) | ((state & LED_STATE_MASK) << id))

Led::Led(uint8_t* pinLeds, uint8_t nLeds) {
    for (uint8_t i=0; i<nLeds && i<LED_MAX; ++i) {
        this->m_leds[i] = pinLeds[i];
        this->m_nLeds++;
    }
}

void Led::init() {
    int i = 0;
	while (i < m_nLeds) {
        int p = m_leds[i];
        pinMode(p, OUTPUT);
        digitalWrite(p, 0);
		++i;
	}
}

bool Led::getState(uint8_t id) {
	return LED_GET_STATE(id);
}

void Led::set(uint8_t id, bool ledState) {
    if (id>=LED_MAX)
        return;
    uint8_t state = ledState ? LED_STATE_MASK : 0x0;
    digitalWrite(m_leds[id], state);
    LED_SET_STATE(id, state);
}

void Led::toggle(uint8_t id) {
    if (id>=LED_MAX)
        return;
    uint8_t state = LED_GET_STATE(id) >> id;
    state = state ^ LED_STATE_MASK;
    digitalWrite(m_leds[id], state);
    LED_SET_STATE(id, state);
}

