/*
 * SM.cpp
 *
 * System Manager State-Machine classes
 *
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 */

#include "SM.hpp"

#include "SM_States.hpp"
#include "cmsis_os.h"
#include "task.h"

namespace SM {

uint32_t AM_PERIOD_MS = 5;  // Current operation speed of 200 Hz.

/*
Helper Functions
*/

AM::AttitudeManagerInput RcToAmInput(LosLinkRx_t rc_message)
{
    AM::AttitudeManagerInput am_message;

    // Get channel values from RC message
    // map pitch/roll/yaw to +/- 30 degrees
    // leave throttle between 0 and 100
    float throttle = rc_message.rx_channels[SM::RC_THROTTLE_CHANNEL] / 20.0;
    float pitch = (rc_message.rx_channels[SM::RC_PITCH_CHANNEL] * 60.0 / 100.0) - 30.0; // So pulling down on stick goes up
    float roll = (rc_message.rx_channels[SM::RC_ROLL_CHANNEL] * 60.0 / 100.0) - 30.0; // RHR with thumb pointed in direction of flight. CW looking forward = positive
    float yaw = (rc_message.rx_channels[SM::RC_YAW_CHANNEL] * 60.0 / 100.0) - 30.0;

    // map RC values into waypoint, assuming 50 for pitch, roll, and yaw means no change desired.
    // sets the maximum 
    am_message.dist_forward = cos(pitch) * cos(roll);
    am_message.dist_right = sin(roll); // Positive right
    am_message.dist_up = sin(pitch); // Positive down, (so pulling controller stick down moves drone up)
    // Note that the above is allready normalized
    am_message.magnitude = throttle;
    am_message.heading = yaw;
    
    return am_message;
}

/*
System Manager Functions
*/

SystemManager::SystemManager()
    : engine{.channel = 0, .state_mix = AM::StateMix(0, 0, 1, 1, 1, -1)},
      left_aileron{.channel = 1, .state_mix = AM::StateMix(0, 0, 1, -1, -1, -1)},
      right_aileron{.channel = 2, .state_mix = AM::StateMix(0, 0, 1, 1, -1, 1)},
      rudder{.channel = 3, .state_mix = AM::StateMix(0, 0, 1, -1, 1, 1)},
      elevator{.channel = 4, .state_mix = AM::StateMix(0, 0, 1, -1, 1, -1)},
      fixed_control(engine, left_aileron, right_aileron, rudder, elevator),
      attitude_manager(&fixed_control) {
    currentState = &BootMode::getInstance();
}

void SystemManager::setState(SystemState& newState) {
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

void SystemManager::AMOperationTask(void* pvParameters) 
{
    AM::AttitudeManager* attitude_manager = (AM::AttitudeManager*)pvParameters;

    const AM::AttitudeManagerInput* am_instructions;
    osStatus_t sm_am_queue_status;

    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        // Read MessageQ from SM
        uint8_t* msg_priority = 0;

        // Arbitrary 3ms timeout as no message is no problem, gives AM time to run
        sm_am_queue_status = osMessageQueueGet(attitude_manager->getSmQueue(), &am_instructions, msg_priority, 3);

        // Clear MessageQ from SM
        osMessageQueueReset(attitude_manager->getSmQueue());

        // if (sm_am_queue_status != osOK) {
        //     // Set a flag for the message to AM once AM has use of it
        // }

        // Run AM control loop
        attitude_manager->runControlLoopIteration(*am_instructions);

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

}  // namespace SM
