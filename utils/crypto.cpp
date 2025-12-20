#include "crypto.h"
#include <CRC32.h>


uint32_t calculateChecksum(const uint8_t* data, size_t size, size_t offset, const uint8_t phrase) {
    CRC32 crc;
    uint8_t *pData = (uint8_t*)data + offset;
    size_t pSize = size - offset;
    if (phrase != 0)
        crc.add(phrase);
    crc.add(pData, pSize);
    return crc.calc();
}
