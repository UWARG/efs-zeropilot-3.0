#include "TM.hpp"


TelemetryManager::TelemetryManager() {

}

void TelemetryManager::Init() {
    
}

TM_Status TelemetryManager::ExecuteTx() {

}

TM_Status TelemetryManager::ExecuteRx() {
    // copy data into local buffer from jetson and RFD LOS buffers 

    // this is temp
    uint8_t rfdBuf[150];
    uint8_t jetsonBuf[150];


    // check the type variable
    uint8_t type = jetsonBuf[3];

    switch(type) {
        case JETSON_RELATIVE_MOVEMENT_COMMAND:
            // handle it
            break;
        case JETSON_LANDING_COMMAND:
            // handle it
            break;
        // not yet sure if jetson commands that are in the wrong direction need to be handled differently or not
        case JETSON_ODOM:
            // we fucked up if the jetson is sending this data
            // fall through
        case JETSON_MOVEMENT_REQ:
            // we fucked up if the jetson is sending this packet
            // fall through
        default:
            // YIKES
            break;
    }


    type = rfdBuf[3];

    switch(type) {
        case GSPC_WAYPOINTS:
        
            break;
        case GSPC_DISARM:
            break;
        case GSPC_PID_VALUES:
            break;
        case GSPC_DATA: 
            // should not be getting this packet 
            // fall through
        case GSPC_PID_SET_RESPONSE:
            // should not be getting this packet
            // fall through
        default:
            // yikes
            break;
    }

    // decode based on the type variable 

    
}

TM_Status TelemetryManager::ExecuteRxJetson() {
    // pull data from LOS and update the queue

    // loop through the bytes in the queue and assemble messages

}