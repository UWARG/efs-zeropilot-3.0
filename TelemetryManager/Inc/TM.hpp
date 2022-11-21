#pragma once

#include "CRC32.hpp"

enum TM_Status {SUCCESS = 0, FAILURE};

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

class TelemetryManager {

    public:
        TelemetryManager();

        TM_Status execute();

        

};