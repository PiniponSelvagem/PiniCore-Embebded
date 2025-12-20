/**
* @file     loracomm.hpp
* @brief    LoRa communication layer.
* @author   PiniponSelvagem
*
* Copyright(C) PiniponSelvagem
*
***********************************************************************
* Software that is described here, is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
**********************************************************************/

#pragma once

#ifndef _PINICORE_LORACOMM_H_
#define _PINICORE_LORACOMM_H_

#include "drivers/communication/lora.hpp"

#define LORACOMM_ONRECEIVE_SIZE_MAX 16  ///> Maximum number of tagIds that can be "subscribed". I want to avoid using 'malloc'.

#define LORACOMM_SEND_PAYLOAD_MAX   (LORA_RECEIVED_PACKET_MAX_SIZE-sizeof(LoRaMessageHeader))   ///> Maximum number of bytes that can be sent, excluding header.
#define LORACOMM_SEND_QUEUE_MAX     8   ///> Maximum number of payloads that can be on the send queue at one time.
#define LORACOMM_SEND_RETRY_MAX     3   ///> Maximum number of retries before dropping if no ACK reply, when required.

//user callbacks
typedef std::function<void(const uint8_t* payload, size_t size, int rssi, float snr)> LoRaOnReceiveCallback;

typedef struct {
    const uint8_t tagId;
    LoRaOnReceiveCallback callback;
} LoRaOnReceiveCallback_t;

typedef struct {
    uint32_t    checksum;
                /**
                 * bit 0 -> sender is Terminal (not Base Station / Gateway)
                 * bit 1 -> requires ACK
                 * bit 2 -> is ACK of a received payload with a particular 'seqId'
                 * bit [3..7] -> reserved
                 * 
                 * Example:
                 * 0b0000_0101
                 * - payload is from a Terminal
                 * - this payload does NOT require an ACK reply/payload
                 * - is an ACK reply/payload to the 'seqId' found in this header
                 */
    uint8_t     flags;
    uint8_t     tagId;
    uint16_t    seqId;
    uint32_t    radioId;
} LoRaHeader_t;

typedef struct {
    bool        requiresACK;
    uint8_t     retryCount;
    uint16_t    seqId;
    uint32_t    radioId;
    uint8_t     payload[LORA_RECEIVED_PACKET_MAX_SIZE]; ///> Includes header.
} LoRaSend_t;


/**
 * TODO:    Should I rethink the decision below? (@warning)
 */
/**
 * @warning All functions of 'LoRaTxRx' can and should be called, except 'onReceive'.
 *          That is because this 'LoRaComm' class sets its own 'LoRaTxRx::onReceive',
 *          replacing any previous 'LoRaTxRx::onReceive' from outside of 'LoRaComm'.
 *          This decision allows outside control of the LoRa hardware, without wrapping
 *          every single function.
 */
class LoRaComm {
    public:
        /**
         * @brief   Initialize the LoRa communication layer.
         * @param   lora Pointer to LoRa hardware driver.
         * @param   isTerminal True when controller is a 'Terminal', false when is a 'Base Station'. Same analogy as a cellular network.
         * @warning The LoRa driver must be initialized before calling this function.
         * @note	This function must be called prior to any other LoraComm functions.
         */
        void init(const LoRaTxRx* lora, bool isTerminal);

        /**
         * @brief   Value used by checksum calculation.
         * @param   phrase A value known by both parties to further improve data validation, if '0' then phrase is not added to checksum calculation.
         */
        void setCryptoPhrase(uint8_t phrase);

        /**
         * @brief   Keeps the LoRa communication alive, if new payload, then calls the appropriate user callback for it.
         * @note    Call this function periodically to parse new received messages.
         */
        void maintain();

        /**
         * @brief   Registers a callback function to be called when the LoRa communication receives a sepecific tagId.
         * @param   tagId Identifies the type of the payload.
         * @param   callback The callback function with the signature void(const uint8_t* payload, size_t size, int rssi, float snr) to be registered.
         * @return  True if there was space in the internal 'm_onReceiveCallbacks' array to add the tagId, false if full and could not be added.
         * @note    Calling this function for same tagId will replace old callback.
         */
        bool onReceive(uint8_t tagId, LoRaOnReceiveCallback callback);

        /**
         * @brief   Unregisters a callback function from being called when the LoRa communication receives a sepecific tagId.
         * @param   tagId The tagId.
         */
        void removeOnReceive(uint8_t tagId);

        /**
         * @brief   Queues a payload to be sent over LoRa.
         * @param   radioId Radio identifier, also known as controller 'serial'.
         * @param   tagId Identifies the type of the payload.
         * @param   payload Payload to be sent.
         * @param   size Size of the payload, up to \ref 'LORACOMM_SEND_PAYLOAD_MAX', if above will truncate.
         * @return  True if payload was queued for send, false if unable because send queue is full.
         */
        bool send(uint32_t radioId, uint8_t tagId, const uint8_t* payload, size_t size);


    private:
        /**
         * @brief   Safely call 'onReceive' callback.
         * @param   tagId Identifies the type of the payload.
         * @param   payload Payload received, excluding header.
         * @param   rssi Signal strenght.
         * @param   snr Signal to noise ratio.
         */
        void _onReceive(uint8_t tagId, const uint8_t* payload, size_t size, int rssi, float snr);


        bool m_isTerminal;      ///> True when controller is a 'Terminal', false when is a 'Base Station'. Same analogy as a cellular network. 
        uint8_t m_cryptoPhrase; ///> Value used to add to the checksum calculation, if '0' then it will not be used and normal checksum will be calculated.
        uint16_t m_seqId;       ///> Current sequence ID, used to remove payloads from send queue when a ACK is received, for the ones that require it.

        /** Send queue **/
        LoRaSend_t m_sendQueue[LORACOMM_SEND_QUEUE_MAX] = {};

        /** Send ACK queue **/
        /**
         * TODO:
         * - this queue should have the RSSI and SNR of the received payload that is going to reply with ACK
         * - if is a Terminal, it should just ACK
         * - if it is a Base Station, it should calculate how long it should wait until it sends a reply ACK
         *      - stronger RSSI and stronger SNR should reply faster
         *      - weaker RSSI and weaker SNR should reply slower
         *      - I think that SNR should have higher bias because the payload was received clearer compared to a weaker SNR
         *      - TIMEOUT should take this time into account, have a margin, and be aware of the payload air time
         *      - It should listen to other Base Station ACK replies, and if is ACK for same Terminal, then drop it and mark is as ACKed
         */

        /** Callbacks **/
        LoRaOnReceiveCallback_t m_onReceiveCallbacks[LORACOMM_ONRECEIVE_SIZE_MAX] = {};

        /** Hardware **/
        LoRaTxRx* m_lora;   ///> Communication hardware.
};

#endif // _PINICORE_LORACOMM_H_