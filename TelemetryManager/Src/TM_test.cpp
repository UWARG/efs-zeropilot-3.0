#include <iostream>
#include <cstdint>

#include "CRC32.hpp"
#include "Messages.hpp"

int main() {

    // test_crc32();


    return 0;
}

void test_decode() {

}

void test_crc32() {

    uint8_t buffer[5] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

    std::cout << "calculating checksum\n";
    uint32_t crc = calculateChecksum(buffer, sizeof(buffer));

    std::cout << std::hex << crc << std::endl;
}