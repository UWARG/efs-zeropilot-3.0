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

void SystemManager::AMOperationTask(void *pvParameters)
{
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        AM_instance.execute();
        vTaskDelayUntil(&xNextWakeTime, SM::AM_PERIOD_MS);
    }
}

void SystemManager::TMOperationTask(void *pvParameters)
{
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        TM_instance.execute();
        vTaskDelayUntil(&xNextWakeTime, SM::TM_PERIOD_OPERATION_MS);
    }
}

void SystemManager::TMSlowTask(void *pvParameters)
{
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        TM_instance.execute();
        vTaskDelayUntil(&xNextWakeTime, SM::TM_PERIOD_SLOW_MS);
    }
}

void SystemManager::PMOperationTask(void *pvParameters)
{
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        PM_instance.execute();
        vTaskDelayUntil(&xNextWakeTime, SM::PM_PERIOD_MS);
    }
}
