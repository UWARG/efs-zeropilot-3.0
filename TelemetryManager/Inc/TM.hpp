#pragma once

#include "CRC32.hpp"
#include "CircularBuffer.hpp"
#include "CommonDataTypes.hpp"

// for now, dont differentiate between different types of failure.
//enum TM_Status {SUCCESS = 0, FAILURE};

// Please ensure this is up to date with Message Formats Doc
enum Msg_Type {
    JETSON_ODOM = 0,
    JETSON_MOVEMENT_REQ,
    JETSON_RELATIVE_MOVEMENT_COMMAND,
    JETSON_LANDING_COMMAND,
    GSPC_WAYPOINTS,
    GSPC_DISARM,
    GSPC_PID_VALUES,
    GSPC_DATA,
    GSPC_PID_SET_RESPONSE
};

const int SIZE_TELEM_BUFFER = 200;
const int SIZE_COMMS_BUFFER = 100;

class TelemetryManager {

    public:

        TelemetryManager();

        void receiveData();
        void init();

    // private:

        uint8_t commsArr[SIZE_COMMS_BUFFER] = { 0 };
        uint8_t telemArr[SIZE_TELEM_BUFFER] = { 0 }; 

        CircularBuffer commsBuffer{commsArr, SIZE_COMMS_BUFFER};
        CircularBuffer telemBuffer{telemArr, SIZE_TELEM_BUFFER};


        void updateCommsBuffer();
        void updateTelemBuffer();
        void parseBuffer(CircularBuffer& buf, SM::TM_SM_Commands& commands);

};

