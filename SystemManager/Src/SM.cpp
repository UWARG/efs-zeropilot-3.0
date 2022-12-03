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
    am_message.x_dir = cos(pitch) * cos(roll);
    am_message.y_dir = sin(roll); // Positive right
    am_message.z_dir = sin(pitch); // Positive down, (so pulling controller stick down moves drone up)
    // Note that the above is allready normalized
    am_message.magnitude = throttle;
    am_message.heading = yaw;
    
    return am_message;
}

SystemManager::SystemManager()
    : back_left_motor{.channel = 0, .stateMix = AM::StateMix(0, 0, 1, 1, 1, -1)},
      front_right_motor{.channel = 1, .stateMix = AM::StateMix(0, 0, 1, -1, -1, -1)},
      back_right_motor{.channel = 2, .stateMix = AM::StateMix(0, 0, 1, 1, -1, 1)},
      front_left_motor{.channel = 3, .stateMix = AM::StateMix(0, 0, 1, -1, 1, 1)},
      quad(back_left_motor, front_right_motor, back_right_motor, front_left_motor),
      attitude_manager(&quad) {
    currentState = &BootMode::getInstance();
}

void SystemManager::setState(SystemState& newState) {
    currentState->exit(this);
    currentState = &newState;
    currentState->enter(this);
}

Drone_Operation_Mode SystemManager::getMode() { return operation_mode; }

void SystemManager::execute() { currentState->execute(this); }

void SystemManager::AMOperationTask(void* pvParameters) {
    // attitude_manager = (AM::AttitudeManager*)pvParameters;

    const AM::AttitudeManagerInput* am_instructions;
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        // Read MessageQ from SM
        uint8_t* msg_priority;
        void* message_pointer;
        // Arbitrary 3ms timeout as no message is no problem, gives AM time to run
        osMessageQueueGet(this->SM_to_AM_queue, message_pointer, msg_priority, 3);
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

}  // namespace SM
