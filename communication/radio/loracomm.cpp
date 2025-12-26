#include "loracomm.hpp"
#include "utils/crypto.hpp"
#include "utils/log.hpp"
#include <string.h>

#define PINICORE_TAG_LORACOMM    "pcore_loracomm"
#define PINICORE_TAG_LORACOMM_CB "pcore_loracomm_cb"

bool LoRaComm::init(
    uint8_t pinMOSI, uint8_t pinMISO, uint8_t pinSCLK, uint8_t pinCS,
    uint8_t pinReset, uint8_t pinDIO0,
    uint16_t carrierFrequency,
    bool isTerminal, uint32_t terminalRadioId
) {
    bool initialized = m_lora.init(pinMOSI, pinMISO, pinSCLK, pinCS, pinReset, pinDIO0, carrierFrequency);
    if (initialized) {
        m_isTerminal = isTerminal;
        m_terminalRadioId = terminalRadioId;
        m_lora.onReceive([this](const uint8_t* payload, size_t size, int rssi, float snr) {
            this->_onReceive(payload, size, rssi, snr);
        });
    }
    return initialized;
}

void LoRaComm::setCryptoPhrase(uint8_t phrase) {
    m_cryptoPhrase = phrase;
}

void LoRaComm::maintain() {
    m_lora.maintain();
}

void LoRaComm::enable() {
    m_lora.enable();
}

void LoRaComm::disable() {
    m_lora.disable();
}

bool LoRaComm::onReceive(uint8_t tagId, LoRaOnReceiveCallback callback) {
    for (int i=0; i<LORACOMM_ONRECEIVE_SIZE_MAX; ++i) {
        LoRaOnReceiveCallback_t* onReceive = &m_onReceiveCallbacks[i];
        if (onReceive->tagId == LORACOMM_INVALID_TAGID) {
            onReceive->tagId    = tagId;
            onReceive->callback = callback;
            return true;
        }
        if (onReceive->tagId == tagId) {
            onReceive->callback = callback;
            return true;
        }
    }
    return false;
}

void LoRaComm::removeOnReceive(uint8_t tagId) {
    for (int i=0; i<LORACOMM_ONRECEIVE_SIZE_MAX; ++i) {
        LoRaOnReceiveCallback_t* onReceive = &m_onReceiveCallbacks[i];
        if (onReceive->tagId == tagId) {
            onReceive->tagId = LORACOMM_INVALID_TAGID;
            return;
        }
    }
}

bool LoRaComm::send(uint32_t radioId, uint8_t tagId, bool requireAck, const uint8_t* payload, size_t size) {
    return _send(radioId, tagId, requireAck, false, payload, size);
}

void LoRaComm::getStatistics(LoRaStatistics_t* stats) {
    if (stats == NULL) return;
    stats->bytesSent       = m_lora.statsBytesSent();
    stats->bytesReceived   = m_lora.statsBytesReceived();
    stats->packetsSent     = m_lora.statsPacketsSent();
    stats->packetsReceived = m_lora.statsPacketsReceived();
}


void LoRaComm::_onReceive(const uint8_t* payload, size_t size, int rssi, float snr) {
    int sizeContent = size-sizeof(LoRaHeader_t);    // size of actual payload, excluding header
    if (
        sizeContent <= 0  // must contain space least 1 byte of usable payload, otherwise goes inside this 'IF'
    ) {
        LOG_T(PINICORE_TAG_LORACOMM_CB, "Received unknown payload: [size: %d] [rssi: %d] [snr: %0.2f]", size, rssi, snr);
        return;
    }

    const uint8_t* payloadContent = payload+sizeof(LoRaHeader_t);
    uint32_t checksum = calculateChecksum(payloadContent, sizeContent, m_cryptoPhrase);
    LoRaHeader_t* header = (LoRaHeader_t*)payload;
    if (header->checksum != checksum) {
        LOG_T(PINICORE_TAG_LORACOMM_CB, "Checksum mismatch: [expected: 0x%x] [actual: 0x%x]", header->checksum, checksum);
        return;
    }

    uint32_t radioId = header->radioId;
    uint8_t tagId = header->tagId;
    LOG_D(PINICORE_TAG_LORACOMM_CB, "Received: [radioId: %d] [tagId: %d] [size: %d] [rssi: %d] [snr: %0.2f]", radioId, tagId, size, rssi, snr);
    if (m_isTerminal && m_terminalRadioId != radioId) {
        return; // Discard payloads not directed to me if I am a Terminal (not a Gateway)
    }
    
    _updateSignalQuality(radioId, rssi, snr);

    bool isAck = (header->flags & (0x1 << LORACOMM_FLAG_IDX_IS_ACK)) != 0;
    if (isAck) {
        LOG_W(PINICORE_TAG_LORACOMM_CB, "ACK received! TODO: handle send queue by removing that 'radioId' and 'checksum' that is in the payload, from the send queue");
        return;
    }
    
    for (int i=0; i<LORACOMM_ONRECEIVE_SIZE_MAX; ++i) {
        LoRaOnReceiveCallback_t* onReceive = &m_onReceiveCallbacks[i];
        if (onReceive->tagId == tagId) {
            onReceive->callback(radioId, payload, size, rssi, snr);
            break;
        }
    }
}

bool LoRaComm::_send(uint32_t radioId, uint8_t tagId, bool requireAck, bool isAck, const uint8_t* payload, size_t size) {
    if (size > LORACOMM_SEND_PAYLOAD_MAX) {
        LOG_W(PINICORE_TAG_LORACOMM, "Send payload too large (%d bytes), max %d bytes", size, LORACOMM_SEND_PAYLOAD_MAX);
        return false;
    }

    uint8_t payloadFull[LORA_PACKET_MAX_SIZE];
    memset(payloadFull, 0, sizeof(payloadFull));
    LoRaHeader_t header;
    size_t sizeFull = sizeof(header)+size;
    header.checksum = calculateChecksum(payload, size, m_cryptoPhrase);
    header.radioId = radioId;
    header.flags =
        ((m_isTerminal ? 1:0) << LORACOMM_FLAG_IDX_IS_TERMINAL) |
        ((requireAck   ? 1:0) << LORACOMM_FLAG_IDX_REQUIRE_ACK) |
        ((isAck        ? 1:0) << LORACOMM_FLAG_IDX_IS_ACK);
    header.tagId = tagId;
    memcpy(payloadFull, &header, sizeof(header));       // place header
    memcpy(payloadFull+sizeof(header), payload, size);  // place content

    LOG_W(PINICORE_TAG_LORACOMM, "TODO: place in a send queue, manage it and handle ACK when requested");
    m_lora.send(payloadFull, sizeFull);
    return true;
}

bool LoRaComm::_sendAck(uint32_t radioId, uint8_t tagId, uint32_t checksumOfReceived) {
    return _send(radioId, tagId, false, true, (uint8_t*)checksumOfReceived, sizeof(checksumOfReceived));
}

void LoRaComm::_updateSignalQuality(uint32_t radioId, int rssi, float snr) {
    if (!m_isTerminal) {
        return; // I am not a Gateway, so ignore all signal quality logic
    }

    uint64_t currMillis = getMillis();
    uint64_t oldest = UINT64_MAX;
    int oldestIdx = 0;   // Guaranteed to be set at least once, but keeping '0' just in case of an edge case
    int selectedIdx = -1;
    LoRaSignalQuality_t* signalQuality;

    // Search for same radioId or oldest element
    for (int i=0; i<LORACOMM_SIGNAL_QUALITY_COUNT_MAX; ++i) {
        signalQuality = &m_signalQuality[i];
        if (signalQuality->lastUpdateAt < oldest) {
            // Found an older element, update the oldestIdx
            oldestIdx = i;
        }
        if (signalQuality->radioId == radioId || signalQuality->lastUpdateAt == 0) {
            // Found same of empty element, select it and use it
            selectedIdx = i;
            break;
        }
    }

    if (selectedIdx != -1) { // Selecting same element, updating its old data
        signalQuality = &m_signalQuality[selectedIdx];
    }
    else { // Could not find any element with same radioId, replace the oldest one in the data structure
        signalQuality = &m_signalQuality[oldestIdx];
    }

    signalQuality->lastUpdateAt = currMillis;
    signalQuality->radioId = radioId;
    signalQuality->rssi = rssi;
    signalQuality->snr = snr;
}

bool LoRaComm::_queueSendAdd(bool requiresACK, uint64_t delay, size_t payloadSize, uint8_t* payload) {
    if (payloadSize > LORA_PACKET_MAX_SIZE) {
        LOG_D(PINICORE_TAG_LORACOMM, "Unable to queue payload for send, payload size too big");
        return false; // payload too big
    }

    uint64_t currMillis = getMillis();
    for (int i=0; i<LORACOMM_SEND_QUEUE_MAX; ++i) {
        LoRaSend_t* sendElement = &m_sendQueue[i];
        if (sendElement->payloadSize == 0) {
            sendElement->requiresACK = requiresACK;
            sendElement->retryCount = 0;
            uint64_t nextRetryAt = currMillis + delay;
            sendElement->nextRetryAt = nextRetryAt;
            sendElement->payloadSize = payloadSize;
            memcpy(sendElement->payload, payload, payloadSize);
            LOG_D(PINICORE_TAG_LORACOMM, "Added to send queue: [payloadSize: %d] [requiresACK: %d] [nextRetryAt: %llu]", payloadSize, requiresACK, nextRetryAt);
            return true; // payload queued for send
        }
    }

    LOG_D(PINICORE_TAG_LORACOMM, "Send queue is full");
    return false; // queue currently full
}

void LoRaComm::_queueSendRemove(LoRaSend_t* sendElement) {
    if (sendElement == NULL) return;
    sendElement->payloadSize = 0;
}

LoRaSend_t* LoRaComm::_queueSendGetReady() {
    uint64_t currMillis = getMillis();
    for (int i=0; i<LORACOMM_SEND_QUEUE_MAX; ++i) {
        LoRaSend_t* sendElement = &m_sendQueue[i];
        if (
            sendElement->payloadSize != 0 &&
            sendElement->nextRetryAt <= currMillis
        ) {
            return &m_sendQueue[i];
        }
    }
    return NULL;
}
