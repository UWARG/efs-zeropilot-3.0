#include "TM.hpp"
#include "LOS_Comms.hpp"
#incliude "Circular_Queue.hpp"

TelemetryManager::TelemetryManager() {

}

void TelemetryManager::updateCommsBuffer() {
    Los_Comms::getInstance()->receive(commsBuffer, SIZE_COMMS_BUFFER, &commsWritePtr, commsReadPtr);
}

void TelemetryManager::updateCommsBuffer() {
    Los_Comms::getInstance()->receive(commsBuffer, SIZE_COMMS_BUFFER, &commsWritePtr, commsReadPtr);
}