#include "PM.hpp"
#include "PM_States.hpp"

PathManager::PathManager()
{
    currentState = &CommsWithAttitude::getInstance();
    status = PathMan::COMPLETED_CYCLE;
    flight_stage = DISARMED;
    isError = false;
}

void PathManager::setState(PathManagerState& newState)
{
    currentState->exit(this);
    currentState = &newState;

    if (*currentState == CommsWithAttitude::getInstance()) {
        status = PathMan::COMPLETED_CYCLE;
    } else if (*currentState == FatalFailureMode::getInstance()) {
        status = PathMan::FAILURE_MODE;
    } else {
        status = PathMan::IN_CYCLE;
    }

    currentState->enter(this);
}

void PathManager::execute()
{
    currentState->execute(this);
}


