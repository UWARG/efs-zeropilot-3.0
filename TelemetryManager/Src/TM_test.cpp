#include <iostream>
#include <cstdint>

#include "CRC32.hpp"

int main() {

    uint8_t buffer[5] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

    std::cout << "calculating checksum\n";
    uint32_t crc = calculateChecksum(buffer, sizeof(buffer));

    std::cout << std::hex << crc << std::endl;

    return 0;
}