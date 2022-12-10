#include "PM.hpp"
#include "PM_States.hpp"

namespace PM {

PathManager::PathManager()
{
    currentState = &CommsWithAttitude::getInstance();
    status = COMPLETED_CYCLE;
    flight_stage = DISARMED;
    isError = false;
}

void PathManager::setState(PathManagerState& newState)
{
    currentState->exit(this);
    currentState = &newState;

    if (*currentState == CommsWithAttitude::getInstance()) {
        status = COMPLETED_CYCLE;
    } else if (*currentState == FatalFailureMode::getInstance()) {
        status = FAILURE_MODE;
    } else {
        status = IN_CYCLE;
    }

    currentState->enter(this);
}

void PathManager::execute()
{
    currentState->execute(this);
}

void PathManager::setSmQueue(osMessageQId queueId) {
    SM_to_PM_queue = queueId;
}

osMessageQId PathManager::getSmQueue() {
    return SM_to_PM_queue;
}

void PathManager::setAmQueue(osMessageQId queueId) {
    PM_to_AM_queue = queueId;
}

osMessageQId PathManager::getAmQueue() {
    return PM_to_AM_queue;
}

}
