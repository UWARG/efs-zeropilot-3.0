/*
 * SM.cpp
 *
 * System Manager State-Machine classes
 *
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 */

#include "../Inc/SM.hpp"
#include "../Inc/SM_States.hpp"

SystemManager::SystemManager()
{
    currentState = &BootMode::getInstance();
}

void SystemManager::setState(SystemState& newState)
{
    currentState->exit(this);
    currentState = &newState;
    currentState->enter(this);
}

void SystemManager::execute()
{
    currentState->execute(this);
}
