#pragma once

#include "CRC32.hpp"

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

    private:

        uint8_t telemBuffer[SIZE_TELEM_BUFFER]; 
        int telemReadPtr = 0;
        int telemWritePtr = 0;

        uint8_t commsBuffer[SIZE_COMMS_BUFFER];
        int commsReadPtr = 0;
        int commsWritePtr = 0;

        void updateCommsBuffer();
        void updateTelemBuffer();

};

