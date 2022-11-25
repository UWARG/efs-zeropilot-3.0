#include <iostream>
#include <cstdint>

#include "CRC32.hpp"
#include "Messages.hpp"

void test_decode();
void test_encode();

int main() {

    // test_crc32();

     test_decode();
    //test_encode();

    return 0;
}

void test_encode() {

    uint8_t buf[150] = { 0 };

    messages::GroundStationDisarm msg;

    // fill in header values
    msg.header.flag = START_FLAG;
    msg.header.length[0] = 0x00;
    msg.header.length[1] = 0x01;
    msg.header.type = 5;

    msg.arm = false;

    // encode message
    msg._encodeNoHash(buf,0,sizeof(messages::GroundStationDisarm));

    // calculate checksum
    uint32_t checksum = calculateChecksum(&buf[3], sizeof(messages::GroundStationDisarm) - 7);
    // checksum will be in little endian
    int endOfMessage = sizeof(messages::GroundStationDisarm) - 1;
    buf[endOfMessage] = (checksum & 0xFF000000) >> 24; 
    buf[endOfMessage - 1] = (checksum & 0x00FF0000) >> 16; 
    buf[endOfMessage - 2] = (checksum & 0x0000FF00) >> 8; 
    buf[endOfMessage - 3] = checksum & 0x000000FF; 

    for (int i = 0; i < sizeof(messages::GroundStationDisarm); i++) {
        std::cout << std::hex << " 0x" << +buf[i];
    }
    std::cout << "\n";
}

// 0x7e 0x0 0x1 0x5 0x0 0xba 0xe6 0xae 0x3c GroundStationDisarm
void test_decode() {
    // it should be in big endian, but were just going to pretend that it came in little endian
    const int maxSize = 150;
    uint8_t buf[maxSize] = {0x7e, 0x00, 0x01, 0x05, 0x00, 0xba, 0xe6, 0xae, 0x3c};


    int size = sizeof(messages::GroundStationDisarm);
    messages::GroundStationDisarm msg;

    // check to make sure the size of the buffer is enough, just in case
    if(size > maxSize) {
        std::cout << "fuck size bad\n"; 
        return;
    }

    // encode message
    msg._decodeNoHash(buf,0,maxSize);

    // calculate checksum
    // the 7 is the number of bytes not included in the checksum (the flag, the length, and the checksum itself)
    uint32_t checksum = calculateChecksum(&buf[3], size - 7);
    // checksum will be in little endian
    int endOfMessage = size - 1;
    uint32_t checksumFromMessage = buf[endOfMessage] << 24;
    checksumFromMessage = checksumFromMessage | (buf[endOfMessage - 1] << 16);
    checksumFromMessage = checksumFromMessage | (buf[endOfMessage - 2] << 8);
    checksumFromMessage = checksumFromMessage | buf[endOfMessage - 3];

    std::cout << std::hex << "calculated checksum: " << checksum << " message checksum: " << checksumFromMessage << "\n";

    if(checksum != checksumFromMessage) {
        msg.header.flag = 0;
        std::cout << "bad checksum\n";
    }
    
    return;
    

}

void test_crc32() {

    uint8_t buffer[5] = {0xaa, 0xaa, 0xaa, 0xaa, 0xaa};

    std::cout << "calculating checksum\n";
    uint32_t crc = calculateChecksum(buffer, sizeof(buffer));

    std::cout << std::hex << crc << std::endl;
}