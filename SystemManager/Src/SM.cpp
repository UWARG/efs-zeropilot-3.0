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

AttitudeManagerInputs SystemManager::RcToAmInput(LosLinkRx_t rc_message)
{
    AttitudeManagerInputs am_message;

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

void SystemManager::AMOperationTask(void *pvParameters)
{
    AttitudeManagerInput am_instructions;
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        // Read MailQ from SM
        osEvent mail_event = osMailGet(this->SM_to_AM_queue, 3); // Arbitrary 3ms timeout as no message is no problem, gives AM time to run
        
        // Clear MailQ from SM
        osMailFree(this->SM_to_AM_queue);

        if (mail_event.status == osEventMail) {
            am_instructions = *mail_event.value.p;
        }

        // Run AM control loop
        this->AM_instance.runControlLoopIteration(am_instructions);

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
