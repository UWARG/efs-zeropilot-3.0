#include "TM.hpp"
#include "LOS_Comms.hpp"
#include "LOS_Telem.hpp"


TelemetryManager::TelemetryManager() {

}

void TelemetryManager::updateCommsBuffer() {
    Los_Comms::getInstance().receive(commsBuffer, SIZE_COMMS_BUFFER, &commsWritePtr, commsReadPtr);
}

void TelemetryManager::updateTelemBuffer() {
    Los_Telem::getInstance().receive(telemBuffer, SIZE_TELEM_BUFFER, &telemWritePtr, telemReadPtr);
}

void TelemetryManager::receiveData() {
    updateCommsBuffer();
}

void TelemetryManager::init() {
    Los_Telem::getInstance().init();
    Los_Comms::getInstance().init();
}