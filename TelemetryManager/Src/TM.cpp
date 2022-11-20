#include "TM.hpp"


TelemetryManager::TelemetryManager() {

}

TM_Status TelemetryManager::execute() {

    // copy data into local buffer from jetson and RFD LOS buffers 

    // this is temp
    uint8_t rfdBuf[150];
    uint8_t jetsonBuf[150];


    // check the type variable

    // handle jetson messages first, then handle GSPC messages   

    //uint8_t type = 

    // decode based on the type variable 

    
}