#pragma once

#ifndef TS_LORACOMM_H
#define TS_LORACOMM_H

#include "global.hpp"
#include "utils/utils.hpp"
#include "common/communication/lora/lora.hpp"
#include "common/communication/icommunication.hpp"

#define TAG_LORACOMM "loracomm"

//#define DEBUG_LORA_PACKET_RAW
//#define DEBUG_LORA_RECEIVED_CONTENT

enum ELoraTag : uint8_t { // uint8_t in use in LoraMessageHeader, ORDER MATTERS!!!
    LT_UNKNOWN,
    LT_DEBUG,
    LT_ACK,
    LT_PING,
    LT_PONG,
    LT_STATUS,
    LT_CONFIG,
    LT_INSTRUCTIONS,
    LT_FLOW,
    LT_SENSOR,
    LT_TEMP_SENSORS,
};



struct LoraMessageHeader {
    uint32_t    checksum;
    uint8_t     tagId;
    uint8_t     senderDeviceType;
    uint16_t    seqId;
    uint32_t    serial;
};

class LoraComm : public ICommunication {
    private:
        void setup() {
            m_lora.setup();
        }
        void sleep() {
            m_lora.sleep();
        }

    public:
        uint16_t send(uint32_t serial, const MessageInstructions *msg) {
            LoraMessageInstructions loraMsg = {};
            loraMsg.header.tagId = ELoraTag::LT_INSTRUCTIONS;
            loraMsg.header.senderDeviceType = p_deviceType;
            loraMsg.header.seqId = m_seqId++;
            loraMsg.header.serial = serial;

            loraMsg.nInstructions = msg->size;
            for (int i=0; i<msg->size && i<COMMMSG_MAX_PROGS; ++i) {
                loraMsg.instList[i].startTime = msg->instList[i].startTime;
                loraMsg.instList[i].duration = msg->instList[i].duration;
                loraMsg.instList[i].weekDays = msg->instList[i].weekDays;
                loraMsg.instList[i].valves = msg->instList[i].valves;
            }

            size_t sizeFilled = (sizeof(loraMsg)-sizeof(loraMsg.instList)) + (sizeof(LoraMessageInstruction)*loraMsg.nInstructions);
            loraMsg.header.checksum = calculateChecksumLoraMsg((char*)&loraMsg, sizeFilled);

            int msgAddedIdx = queueSend(loraMsg.header.seqId, true, (char*)(&loraMsg), sizeFilled);
            if (msgAddedIdx != -1) {
                INFO(TAG_LORACOMM,
                    "Message send queued at index %d: LT_INSTRUCTIONS [serial=%d] [nInstructions=%d]",
                    msgAddedIdx, serial, msg->size
                );
            }
            return loraMsg.header.seqId;
        }
        uint16_t send(uint32_t serial, const MessageConfig *msg) {
            LoraMessageConfig loraMsg = {};
            loraMsg.header.tagId = ELoraTag::LT_CONFIG;
            loraMsg.header.senderDeviceType = p_deviceType;
            loraMsg.header.seqId = m_seqId++;
            loraMsg.header.serial = serial;

            loraMsg.timestamp = msg->timestamp;
            loraMsg.communicationFrequency = msg->commFreq;
            loraMsg.valveDelay = msg->valveDelay;

            loraMsg.header.checksum = calculateChecksumLoraMsg((char*)&loraMsg, sizeof(loraMsg));

            int msgAddedIdx = queueSend(loraMsg.header.seqId, true, (char*)(&loraMsg), sizeof(loraMsg));
            if (msgAddedIdx != -1) {
                INFO(TAG_LORACOMM,
                    "Message send queued at index %d: LT_CONFIG [serial=%d] [timestamp=%lli] [commFreq=%d] [valveDelay=%d]",
                    msgAddedIdx, serial, msg->timestamp, msg->commFreq, msg->valveDelay
                );
            }
            return loraMsg.header.seqId;
        }
        uint16_t send(uint32_t serial, const MessageStatus *msg) {
            LoraMessageStatus loraMsg = {};
            loraMsg.header.tagId = ELoraTag::LT_STATUS;
            loraMsg.header.senderDeviceType = p_deviceType;
            loraMsg.header.seqId = m_seqId++;
            loraMsg.header.serial = serial;

            loraMsg.rssi = msg->rssi;
            loraMsg.firmwareVersion = msg->firmwareVersion;
            loraMsg.nInstructions = msg->nInstructions;
            loraMsg.askFor = msg->askFor;
            loraMsg.timestamp = msg->timestamp;
            loraMsg.uptime = msg->uptime;
            loraMsg.communicationFrequency = msg->communicationFrequency;
            loraMsg.valveDelay = msg->valveDelay;
            loraMsg.battery = msg->battery;

            loraMsg.header.checksum = calculateChecksumLoraMsg((char*)&loraMsg, sizeof(loraMsg));

            int msgAddedIdx = queueSend(loraMsg.header.seqId, true, (char*)(&loraMsg), sizeof(loraMsg));
            if (msgAddedIdx != -1) {
                INFO(TAG_LORACOMM,
                    "Message send queued at index %d: LT_STATUS [serial=%d] [nInstructions=%d] [battery=%d]",
                    msgAddedIdx, serial, msg->nInstructions, msg->battery
                );
            }
            return loraMsg.header.seqId;
        }
        uint16_t send(uint32_t serial, const MessageDebug *msg) {
            LoraMessageDebug loraMsg = {};
            loraMsg.header.tagId = ELoraTag::LT_DEBUG;
            loraMsg.header.senderDeviceType = p_deviceType;
            loraMsg.header.seqId = m_seqId++;
            loraMsg.header.serial = serial;

            loraMsg.value = msg->value;

            loraMsg.header.checksum = calculateChecksumLoraMsg((char*)&loraMsg, sizeof(loraMsg));
            int msgAddedIdx = queueSend(loraMsg.header.seqId, true, (char*)(&loraMsg), sizeof(loraMsg));
            if (msgAddedIdx != -1) {
                INFO(TAG_LORACOMM, "Message send queued at index %d: LT_DEBUG [serial=%d] [value=%d]", msgAddedIdx, serial, msg->value);
            }
            return loraMsg.header.seqId;
        }
        uint16_t send(uint32_t serial, const MessagePing *msg) {
            LoraMessagePing loraMsg = {};
            loraMsg.header.tagId = ELoraTag::LT_PING;
            loraMsg.header.senderDeviceType = p_deviceType;
            loraMsg.header.seqId = m_seqId++;
            loraMsg.header.serial = serial;

            loraMsg.header.checksum = calculateChecksumLoraMsg((char*)&loraMsg, sizeof(loraMsg));
            int msgAddedIdx = queueSend(loraMsg.header.seqId, false, (char*)(&loraMsg), sizeof(loraMsg));
            if (msgAddedIdx != -1) {
                INFO(TAG_LORACOMM, "Message send queued at index %d: LT_PING [serial=%d]", msgAddedIdx, serial);
            }
            return loraMsg.header.seqId;
        }
        uint16_t send(uint32_t serial, const MessagePong *msg) {
            LoraMessagePong loraMsg = {};
            loraMsg.header.tagId = ELoraTag::LT_PONG;
            loraMsg.header.senderDeviceType = p_deviceType;
            loraMsg.header.seqId = m_seqId++;
            loraMsg.header.serial = serial;
            
            loraMsg.serial = msg->serial;
            loraMsg.rssi = msg->rssi;
            loraMsg.snr = msg->snr;

            loraMsg.header.checksum = calculateChecksumLoraMsg((char*)&loraMsg, sizeof(loraMsg));
            int msgAddedIdx = queueSend(loraMsg.header.seqId, false, (char*)(&loraMsg), sizeof(loraMsg));
            if (msgAddedIdx != -1) {
                INFO(TAG_LORACOMM, "Message send queued at index %d': LT_PONG [serial=%d]", msgAddedIdx, serial);
            }
            return loraMsg.header.seqId;
        }

    protected:
        void send(const char* msg, size_t size) {
            m_lora.send((uint8_t*)msg, size);
            
            /* Statistics */
            statsUpdateSent(size);
        }

        bool receive(ReceiveInfo *rcvInfo) {
            char msg[COMMMSG_MAX_MSG_SIZE];

            LoRaReceived loraReceived = m_lora.receive((char*)msg, COMMMSG_MAX_MSG_SIZE);
            size_t size = loraReceived.size;
            rcvInfo->rssi = loraReceived.rssi;
            rcvInfo->snr = loraReceived.snr;

            // Handle received message
            if (size != 0) {
                processReceived(msg, size, rcvInfo);
                //TRACE(TAG_LORACOMM, "ReceiveInfo [seqId=%d] [requiresACK=%s]", rcvInfo->seqId, rcvInfo->requiresACK?"true":"false");
                return true;
            }
            return false;
        }

    private:
        uint32_t calculateChecksumLoraMsg(const char* msg, size_t size) {
            return calculateChecksum(msg, size, sizeof(LoraMessageHeader::checksum));
        }

        void processReceived(const char* msg, size_t size, ReceiveInfo *rcvInfo) {
            /* Statistics */
            statsUpdateRcvd(size);

        #ifdef DEBUG_LORA_PACKET_RAW
            TRACE(TAG_LORACOMM, "LoRa Raw Packet:")
            for (size_t j=0; j<size; ++j) {
                if (msg[j] < 16) Serial.print("0");
                Serial.print(msg[j], HEX);
                Serial.print(" ");
            }
            Serial.println();
        #endif
            bool isFromTrigger = isMsgFromTrigger(msg, size);
        #ifdef DEBUG_LORA_PACKET_RAW
            TRACE(TAG_LORACOMM, "Received packet. From Trigger? %s", isFromTrigger?"true":"false");
        #endif
            if (isFromTrigger) {
                bool isForMe = 
                    p_msgReceiver->isReceivedForMe(((LoraMessageHeader*)msg)->serial) &&
                    !isMsgFromSameDeviceType((EDeviceType)((LoraMessageHeader*)msg)->senderDeviceType)
                ;
            #ifdef DEBUG_LORA_PACKET_RAW
                TRACE(TAG_LORACOMM, "Received packet. For me? %s", isForMe?"true":"false");
            #endif
                if (isForMe) {
                    /* Statistics */
                    statsUpdateRcvdValid(size);

                    return loraHandleReceivedMessage(
                        msg,
                        size,
                        rcvInfo
                    );
                }
            }

            // not for me, no seqId, ignore it
            rcvInfo->seqId = -1;
            rcvInfo->requiresACK = false;
        }

        bool recreateRadioMessage(const char* data, size_t dataSize, void *pStruct, size_t structSize, bool canBeShort = false) {
            if (
                (dataSize != structSize && !canBeShort) ||
                (dataSize  > structSize &&  canBeShort)
            ) {
                // The received message does not match the target LoRa tag id
                ERROR(
                    TAG_LORACOMM,
                    "Unable to 'recreateRadioMessage', %s dataSize is %d bytes but got %d bytes",
                    canBeShort?"maximum":"expected", structSize, dataSize
                );
                return false;
            }
            memcpy(pStruct, data, structSize);
            return true;
        }

        bool isMsgFromSameDeviceType(EDeviceType senderDeviceType) { return senderDeviceType == p_deviceType; }
        bool isMsgFromTrigger(const char *msg, size_t size) {
            if (size < sizeof(LoraMessageHeader)) {
                // Definitely it is not a Trigger.Systems LoRa message
                return false;
            }

            LoraMessageHeader *data = (LoraMessageHeader*) msg;
            if (data->checksum != calculateChecksumLoraMsg(msg, size)) {
                // Looked like a valid message, but failed during checksum
                return false;
            }
            return true;
        }

        void loraHandleReceivedMessage(const char* msg, size_t size, ReceiveInfo *rcvInfo) {
            rcvInfo->seqId = -1;
            rcvInfo->requiresACK = true;
            bool msgRecreated = false;
            
            ELoraTag loraTagReceived = (ELoraTag) ((LoraMessageHeader*)msg)->tagId;

        #ifndef DEBUG_LORA_RECEIVED_CONTENT
            uint32_t serial = ((LoraMessageHeader*)msg)->serial;
            INFO(TAG_LORACOMM, "Received: ELoraTag::%d [serial=%d]", loraTagReceived, serial);
        #endif

            switch (loraTagReceived) {
                case ELoraTag::LT_UNKNOWN: {
                    WARN(TAG_LORACOMM, "ELoraTag::LT_UNKNOWN");
                    break;
                }
                case ELoraTag::LT_INSTRUCTIONS: {
                    LoraMessageInstructions rMsg;
                    msgRecreated = recreateRadioMessage(msg, size, &rMsg, sizeof(rMsg), true);
                    if (msgRecreated) {
                    #ifdef DEBUG_LORA_RECEIVED_CONTENT
                        DEBUG(
                            TAG_LORACOMM,
                            "LoraMessageInstructions -> { H.tagId=%d, H.sedId=%d, H.checksum=0x%x, H.serial='%u', "
                            "nInstructions=%d, "
                            "instList=<'Optimized Out'> }",
                            rMsg.header.tagId, rMsg.header.seqId, rMsg.header.checksum, rMsg.header.serial,
                            rMsg.nInstructions
                        );
                    #endif
                        MessageInstructions msg = {};
                        msg.size = rMsg.nInstructions;
                        
                        for (int i=0; i<rMsg.nInstructions && i<COMMMSG_MAX_PROGS; ++i) {
                            msg.instList[i].startTime = rMsg.instList[i].startTime;
                            msg.instList[i].duration = rMsg.instList[i].duration;
                            msg.instList[i].weekDays = rMsg.instList[i].weekDays;
                            msg.instList[i].valves = rMsg.instList[i].valves;
                        }
                        uint32_t instListChecksum = p_msgReceiver->onReceivedInstructions((const MessageInstructions*) &msg);

                        time_t timestamp = p_msgReceiver->onGetTimestamp();
                        rcvInfo->seqId = rMsg.header.seqId;
                        DataChecksum checksums;
                        p_msgReceiver->onGetChecksumsSaved(rMsg.header.serial, &checksums);
                        sendAck(rMsg.header.serial, rMsg.header.seqId, timestamp, checksums);
                    }
                    else { ERROR(TAG_LORACOMM, "Unable to 'recreateRadioMessage' for LT_INSTRUCTIONS"); }
                    break;
                }
                case ELoraTag::LT_CONFIG: {
                    LoraMessageConfig rMsg;
                    msgRecreated = recreateRadioMessage(msg, size, &rMsg, sizeof(rMsg));
                    if (msgRecreated) {
                    #ifdef DEBUG_LORA_RECEIVED_CONTENT
                        DEBUG(
                            TAG_LORACOMM,
                            "LoraMessageConfig -> { H.tagId=%d, H.seqId=%d, H.checksum=0x%x, H.serial='%u', "
                            "timestamp=%li, "
                            /*"communicationStartTime=%d, */"communicationFrequency=%d, "/*communicationInterval=%d, "*/
                            "valveDelay=%d }", /*, batteryLimits=%d, offset=%d }",*/
                            rMsg.header.tagId, rMsg.header.seqId, rMsg.header.checksum, rMsg.header.serial,
                            rMsg.timestamp,
                            /*rMsg.communicationStartTime, */rMsg.communicationFrequency,/* rMsg.communicationInterval,*/
                            rMsg.valveDelay/*, rMsg.batteryLimits, rMsg.offset*/
                        );
                    #endif
                        MessageConfig msg = {};
                        msg.timestamp = rMsg.timestamp;
                        msg.commFreq = rMsg.communicationFrequency;
                        msg.valveDelay = rMsg.valveDelay;
                        uint32_t configChecksum = p_msgReceiver->onReceivedConfig((const MessageConfig*) &msg);
                        
                        time_t timestamp = p_msgReceiver->onGetTimestamp();
                        rcvInfo->seqId = rMsg.header.seqId;
                        DataChecksum checksums;
                        p_msgReceiver->onGetChecksumsSaved(rMsg.header.serial, &checksums);
                        sendAck(rMsg.header.serial, rMsg.header.seqId, timestamp, checksums);
                    }
                    else { ERROR(TAG_LORACOMM, "Unable to 'recreateRadioMessage' for LT_CONFIG"); }
                    break;
                }
                case ELoraTag::LT_STATUS: {
                    LoraMessageStatus rMsg;
                    msgRecreated = recreateRadioMessage(msg, size, &rMsg, sizeof(rMsg));
                    if (msgRecreated) {
                    #ifdef DEBUG_LORA_RECEIVED_CONTENT
                        DEBUG(
                            TAG_LORACOMM,
                            "LoraMessageStatus -> { H.tagId=%d, H.seqId=%d, H.checksum=0x%x, H.serial='%u', "
                            "rssi=%d, firmwareVersion=%d, nInstructions=%d, "
                            "askFor=%d, timestamp=%li, uptime=%li, "
                            "communicationFrequency=%d, valveDelay=%d, battery=%d }",
                            rMsg.header.tagId, rMsg.header.seqId, rMsg.header.checksum, rMsg.header.serial,
                            rMsg.rssi, rMsg.firmwareVersion, rMsg.nInstructions,
                            rMsg.askFor, rMsg.timestamp, rMsg.uptime,
                            rMsg.communicationFrequency, rMsg.valveDelay, rMsg.battery
                        );
                    #endif
                        MessageStatus msg = {};
                        msg.rssi = rMsg.rssi;
                        msg.firmwareVersion = rMsg.firmwareVersion;
                        msg.nInstructions = rMsg.nInstructions;
                        msg.askFor = (EAskFor)rMsg.askFor,
                        msg.timestamp = rMsg.timestamp;
                        msg.uptime = rMsg.uptime;
                        msg.communicationFrequency = rMsg.communicationFrequency;
                        msg.valveDelay = rMsg.valveDelay;
                        msg.battery = rMsg.battery;

                        p_msgReceiver->onReceivedStatus(rMsg.header.serial, (const MessageStatus*) &msg);
                        rcvInfo->seqId = rMsg.header.seqId;

                        time_t timestamp = p_msgReceiver->onGetTimestamp();
                        DataChecksum checksums;
                        p_msgReceiver->onGetChecksumsSaved(rMsg.header.serial, &checksums);
                        sendAck(rMsg.header.serial, rMsg.header.seqId, timestamp, checksums);
                    }
                    else { ERROR(TAG_LORACOMM, "Unable to 'recreateRadioMessage' for LT_STATUS"); }
                    break;
                }
                case ELoraTag::LT_ACK: {
                    LoraMessageACK rMsg;
                    msgRecreated = recreateRadioMessage(msg, size, &rMsg, sizeof(rMsg));
                    if (msgRecreated) {
                    #ifdef DEBUG_LORA_RECEIVED_CONTENT
                        DEBUG(
                            TAG_LORACOMM,
                            "LoraMessageACK -> { H.tagId=%d, H.seqId=%d, H.checksum=0x%x, H.serial='%u', "
                            "timestamp=%li, C.config=0x%x, C.instList=0x%x }",
                            rMsg.header.tagId, rMsg.header.seqId, rMsg.header.checksum, rMsg.header.serial,
                            rMsg.timestamp, rMsg.checksums.config, rMsg.checksums.instList
                        );
                    #endif
                        MessageAck msg = {};
                        msg.timestamp = rMsg.timestamp;
                        msg.checksums.config = rMsg.checksums.config;
                        msg.checksums.instList = rMsg.checksums.instList;
                        p_msgReceiver->onReceivedAck(rMsg.header.serial, (const MessageAck*) &msg);
                        rcvInfo->seqId = rMsg.header.seqId;
                        rcvInfo->requiresACK = false;
                    }
                    else { ERROR(TAG_LORACOMM, "Unable to 'recreateRadioMessage' for LT_ACK"); }
                    break;
                }
                case ELoraTag::LT_FLOW: {
                    WARN(TAG_LORACOMM, "ELoraTag::RT_FLOW");
                    break;
                }
                case ELoraTag::LT_SENSOR: {
                    WARN(TAG_LORACOMM, "ELoraTag::LT_SENSOR");
                    break;
                }
                case ELoraTag::LT_TEMP_SENSORS: {
                    WARN(TAG_LORACOMM, "ELoraTag::LT_TEMP_SENSORS");
                    break;
                }
                case ELoraTag::LT_DEBUG: {
                    LoraMessageDebug rMsg;
                    msgRecreated = recreateRadioMessage(msg, size, &rMsg, sizeof(rMsg));
                    if (msgRecreated) {
                    #ifdef DEBUG_LORA_RECEIVED_CONTENT
                        DEBUG(
                            TAG_LORACOMM,
                            "LoraMessageDebug -> { H.tagId=%d, H.seqId=%d, H.checksum=0x%x, H.serial='%u', value=%d }",
                            rMsg.header.tagId, rMsg.header.seqId, rMsg.header.checksum, rMsg.header.serial, rMsg.value
                        );
                    #endif
                        MessageDebug msg = {};
                        msg.value = rMsg.value;
                        p_msgReceiver->onReceivedDebug(rMsg.header.serial, &msg);
                        rcvInfo->seqId = rMsg.header.seqId;

                        time_t timestamp = p_msgReceiver->onGetTimestamp();
                        DataChecksum checksums;
                        p_msgReceiver->onGetChecksumsSaved(rMsg.header.serial, &checksums);
                        sendAck(rMsg.header.serial, rMsg.header.seqId, timestamp, checksums);
                    }
                    else { ERROR(TAG_LORACOMM, "Unable to 'recreateRadioMessage' for LT_DEBUG"); }
                    break;
                }
                case ELoraTag::LT_PING: {
                    LoraMessagePing rMsg;
                    msgRecreated = recreateRadioMessage(msg, size, &rMsg, sizeof(rMsg));
                    if (msgRecreated) {
                    #ifdef DEBUG_LORA_RECEIVED_CONTENT
                        DEBUG(
                            TAG_LORACOMM,
                            "LoraMessagePing -> { H.tagId=%d, H.seqId=%d, H.checksum=0x%x, H.serial='%u' }",
                            rMsg.header.tagId, rMsg.header.seqId, rMsg.header.checksum, rMsg.header.serial
                        );
                    #endif
                        MessagePing msg = {};
                        p_msgReceiver->onReceivedPing(rMsg.header.serial, &msg, rcvInfo->rssi, rcvInfo->snr);
                        rcvInfo->seqId = rMsg.header.seqId;
                        rcvInfo->requiresACK = false;
                    }
                    else { ERROR(TAG_LORACOMM, "Unable to 'recreateRadioMessage' for LT_PING"); }
                    break;
                }
                case ELoraTag::LT_PONG: {
                    LoraMessagePong rMsg;
                    msgRecreated = recreateRadioMessage(msg, size, &rMsg, sizeof(rMsg));
                    if (msgRecreated) {
                    #ifdef DEBUG_LORA_RECEIVED_CONTENT
                        DEBUG(
                            TAG_LORACOMM,
                            "LoraMessagePong -> { H.tagId=%d, H.seqId=%d, H.checksum=0x%x, H.serial='%u', "
                            "serial=%d rssi=%d snr=%.2f }",
                            rMsg.header.tagId, rMsg.header.seqId, rMsg.header.checksum, rMsg.header.serial,
                            rMsg.serial, rMsg.rssi, rMsg.snr
                        );
                    #endif
                        MessagePong msg = {};
                        msg.serial = rMsg.serial;
                        msg.rssi = rMsg.rssi;
                        msg.snr = rMsg.snr;
                        p_msgReceiver->onReceivedPong(rMsg.header.serial, &msg, rcvInfo->rssi, rcvInfo->snr);
                        rcvInfo->seqId = rMsg.header.seqId;
                        rcvInfo->requiresACK = false;
                    }
                    else { ERROR(TAG_LORACOMM, "Unable to 'recreateRadioMessage' for LT_PONG"); }
                    break;
                }

                default:
                    DEBUG(TAG_LORACOMM, "ELoraTag (%d) received not implemented", loraTagReceived);
                    break;
            }
        }

        void sendAck(uint32_t serial, uint16_t seqId, time_t timestamp, DataChecksum checksums) {
            LoraMessageACK msg;
            msg.header.tagId = ELoraTag::LT_ACK;
            msg.header.senderDeviceType = p_deviceType;
            msg.header.seqId = seqId;
            msg.header.serial = serial;
            msg.timestamp = timestamp;
            msg.checksums.config = checksums.config;
            msg.checksums.instList = checksums.instList;
            msg.header.checksum = calculateChecksumLoraMsg((char*)&msg, sizeof(msg));
            int msgAddedIdx = queueSend(msg.header.seqId, false, (char*)&msg, sizeof(msg));
            INFO(TAG_LORACOMM, "Message send queued at index %d: LT_ACK [seqId=%d] [serial=%d]", msgAddedIdx, seqId, serial);
        }



        /* Communication */
        LoRaTxRx m_lora;

        uint16_t m_seqId = 0;   // sequence ID used back on ACK which then is used to remove from my send queue messages
};

#endif // TS_LORACOMM_H