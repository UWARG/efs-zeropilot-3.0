#include "TM.hpp"
#include "LOS_Comms.hpp"
#include "LOS_Telem.hpp"
#include "Messages.h"
#include <cstring>


TelemetryManager::TelemetryManager() {

}

void TelemetryManager::updateCommsBuffer() {
    Los_Comms::getInstance().receive(commsBuffer);
}

void TelemetryManager::updateTelemBuffer() {
    Los_Telem::getInstance().receive(telemBuffer);
}

void TelemetryManager::parseBuffer(CircularBuffer& buf, SM::TM_SM_Commands& commands) {
    // 7 is the amount of the overhead, 145 is the size of the longest message
    uint8_t tempBuf[8 + 145] = { 0 };
    uint8_t byte = 0;
    uint8_t status = buf.peek(byte, 0);
    if(status == 0) return;

    // start byte
    if(byte != 0x7e) {
        // data is messed up i suppose
        // i should loop through the buffer until i see a 0x7e
        return;
    }

    // length byte
    uint16_t length = 0;
    status = buf.peek(byte, 1);
    if(status == 0) return;
    length = (uint16_t)byte << 8;

    status = buf.peek(byte, 2);
    if(status == 0) return;

    length = length | (uint16_t)byte;

    // check the existing length based on type and confirm to ensure its good
    // status = buf.peek(byte, 2);
    // if(status == 0) return;

    uint8_t type = 0xFF;
    status = buf.peek(type, 3);
    if(status == 0) return;

    // check if the entire message is in the buffer
    // current byte is the position of start flag, the length bytes, type byte
    int size = 1 + 2 + 1 + length + 4;
    status = buf.read(tempBuf,size);
    if(status == 0) return;

    switch(type) {
        case GSPC_DISARM:
            // TODO: make these functions use references rather than just setting the flag to 0
            TelemMessages::GroundStationDisarm msg = helpers::decodeGroundStationDisarm(tempBuf,size);
            if(msg.header.flag == 0) {
                // the data is all fucked 
                return;
            }
            commands.ground_station_disarm_command.set_armed = msg.arm;
            break;

    }
    
}

void TelemetryManager::receiveData() {
    uint8_t temp[SIZE_COMMS_BUFFER];
    memcpy(temp,commsArr, SIZE_COMMS_BUFFER);
    updateCommsBuffer();
    updateTelemBuffer();

    SM::TM_SM_Commands commands;

    parseBuffer(commsBuffer,commands);
}

void TelemetryManager::init() {
    Los_Telem::getInstance().init();
    Los_Comms::getInstance().init();
}
