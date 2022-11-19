#include "CRC32.hpp"

// shamelessly stolen from https://en.wikipedia.org/wiki/Cyclic_redundancy_check#CRC-32_algorithm
uint32_t calculateChecksum(uint8_t* buf, uint8_t size) {

    uint32_t crc = 0xFFFFFFFF;

    for (int i = 0; i < size; i++) {
        uint8_t lookup = (crc ^ buf[i]) & 0xFF;
        crc = (crc >> 8) ^ crcTable[lookup];
    }

    crc = crc ^ 0xFFFFFFFF;

    return crc;
}