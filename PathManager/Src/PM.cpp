#include "PM.hpp"
#include "PM_States.hpp"

pathManager::pathManager()
{
    currentState = &commsWithAttitude::getInstance();
    status = PathMan::COMPLETED_CYCLE;
    flight_stage = DISARMED;
    isError = false;
}

void pathManager::setState(pathManagerState& newState)
{
    currentState->exit(this);
    currentState = &newState;

    if (*currentState == commsWithAttitude::getInstance()) {
        status = PathMan::COMPLETED_CYCLE;
    } else if (*currentState == fatalFailureMode::getInstance()) {
        status = PathMan::FAILURE_MODE;
    } else {
        status = PathMan::IN_CYCLE;
    }

    currentState->enter(this);
}

void pathManager::execute()
{
    currentState->execute(this);
}


