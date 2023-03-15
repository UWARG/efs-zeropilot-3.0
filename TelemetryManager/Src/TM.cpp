#include "TM.hpp"
#include "LOS_Comms.hpp"
#include "LOS_Telem.hpp"
#include <cstring>


TelemetryManager::TelemetryManager() {

}

void TelemetryManager::updateCommsBuffer() {
    Los_Comms::getInstance().receive(commsBuffer, SIZE_COMMS_BUFFER, commsWritePtr, commsReadPtr);
}

void TelemetryManager::updateTelemBuffer() {
    Los_Telem::getInstance().receive(telemBuffer, SIZE_TELEM_BUFFER, telemWritePtr, telemReadPtr);
}

uint8_t TelemetryManager::peekCommsBuffer(uint8_t& res, int dist) {
    return peekBuffer(commsBuffer, SIZE_COMMS_BUFFER, commsReadPtr, commsWritePtr, res, dist);
}

uint8_t TelemetryManager::peekTelemBuffer(uint8_t& res, int dist) {
    return peekBuffer(telemBuffer, SIZE_TELEM_BUFFER, telemReadPtr, telemWritePtr, res, dist);
}


void parseBuffer(CircularBuffer buf) {

    // 7 is the amount of the overhead, 145 is the size of the longest message
    uint8_t tempBuf[8 + 145]
    uint8_t byte = 0;
    uint8_t status = buf.peek(byte, 0);
    if(status == 0) return;

    // start byte
    if(byte != 0x7e) {
        // data is messed up i suppose
        return;
    }

    // length byte
    uint16_t length = 0;
    uint8_t status = buf.peek(byte, 1);
    if(status == 0) return;
    length = (uint16_t)byte << 8;

    uint8_t status = buf.peek(byte, 2);
    if(status == 0) return;

    length = length & (uint16_t)byte;

    // check the existing length based on type and confirm to ensure its good
    uint8_t status = buf.peek(byte, 2);
    if(status == 0) return;

    // check if the entire message is in the buffer

    // current byte is the position of start flag, the length bytes, type byte
    status = buf.read(tempBuffer,1 + 2 + 1 + length + 4)
    if(status == 0) return;

    
}

void TelemetryManager::receiveData() {
    uint8_t temp[SIZE_COMMS_BUFFER];
    memcpy(temp,commsBuffer, SIZE_COMMS_BUFFER);
    updateCommsBuffer();
    updateTelemBuffer();
}

void TelemetryManager::init() {
    Los_Telem::getInstance().init();
    Los_Comms::getInstance().init();
}
