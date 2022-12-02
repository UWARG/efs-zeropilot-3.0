/*
 * SM.cpp
 *
 * System Manager State-Machine classes
 *
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 */

#include "cmsis_os.h"
#include "SM.hpp"
#include "SM_States.hpp"
#include "task.h"

namespace SM {
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

Drone_Operation_Mode SystemManager::getMode()
{
    return operation_mode;
}

void SystemManager::execute()
{
    currentState->execute(this);
}

void SystemManager::AMOperationTask(void *pvParameters)
{
    attitude_manager = (AM::AttitudeManager*)pvParameters;
    
    const AM::AttitudeManagerInput* am_instructions;
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        // Read MessageQ from SM
        uint8_t* msg_priority;
        void* message_pointer;
        osMessageQueueGet(this->SM_to_AM_queue, message_pointer, msg_priority, 3); // Arbitrary 3ms timeout as no message is no problem, gives AM time to run
        am_instructions = (AM::AttitudeManagerInput*)message_pointer;

        // Clear MessageQ from SM
        osMessageQueueReset(this->SM_to_AM_queue);

        // Run AM control loop
        attitude_manager.runControlLoopIteration(*am_instructions);

        // Delay to operate at set frequency
        vTaskDelayUntil(&xNextWakeTime, SM::AM_PERIOD_MS);
    }
}

// void SystemManager::TMOperationTask(void *pvParameters)
// {
//     TickType_t xNextWakeTime;
//     xNextWakeTime = xTaskGetTickCount();
//     while (true) {
//         TM_instance.execute();
//         vTaskDelayUntil(&xNextWakeTime, SM::TM_PERIOD_OPERATION_MS);
//     }
// }

// void SystemManager::TMSlowTask(void *pvParameters)
// {
//     TickType_t xNextWakeTime;
//     xNextWakeTime = xTaskGetTickCount();
//     while (true) {
//         TM_instance.execute();
//         vTaskDelayUntil(&xNextWakeTime, SM::TM_PERIOD_SLOW_MS);
//     }
// }

// void SystemManager::PMOperationTask(void *pvParameters)
// {
//     TickType_t xNextWakeTime;
//     xNextWakeTime = xTaskGetTickCount();
//     while (true) {
//         PM_instance.execute();
//         vTaskDelayUntil(&xNextWakeTime, SM::PM_PERIOD_MS);
//     }
// }

} // namespace SM
