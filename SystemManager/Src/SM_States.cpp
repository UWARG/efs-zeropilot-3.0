/*
 * SM_States.cpp
 *
 * System Manager State-Machine classes
 *
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 */

#include "SM_States.hpp"

#include "LOS_Actuators.hpp"
#include "LOS_Link.hpp"
#include "config.hpp"
#include "task.h"

namespace SM {

/********************
 * Boot Mode
 ********************/
void BootMode::execute(SystemManager *system_manager) {
    // Only Executes once, Configure sensors, LOS, Everything startup
    // Don't think Enter is used for boot.

    // Start TM at slow speed
    // Update TM mail queue ID
    // system_manager->TM_to_SM_queue = osMessageQueueNew(1, 256, NULL);
    // xTaskCreate(system_manager->TMSlowTask, "TM Slow Thread", &system_manager->TM_handle);
    // vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked

    // Immediately move to Disarm mode
    system_manager->setState(DisarmMode::getInstance());
}

SystemState &BootMode::getInstance() {
    static BootMode singleton;
    return singleton;
}

/********************
 * Disarm Mode
 ********************/
void DisarmMode::execute(SystemManager *system_manager) {
    // TODO Check if we received new PID tunings and call the AM function here

    // Check all arming mechnisms triggered then move to GroundOp
    // TODO

    // Check switch here that changes modes
    system_manager->rc_data = Los_Link::getInstance().getRx();
    if (system_manager->rc_data.rx_channels[SM::RC_ARM_CHANNEL] > 90) {
        system_manager->setState(GroundOpMode::getInstance());
    }
}

SystemState &DisarmMode::getInstance() {
    static DisarmMode singleton;
    return singleton;
}

/********************
 * Ground Op Mode
 ********************/
void GroundOpMode::execute(SystemManager *system_manager) {
    // To move to flight check that armed switch is high,
    // throttle and pitch are 0 (both sticks down)
    system_manager->rc_data = Los_Link::getInstance().getRx();
    if (system_manager->rc_data.rx_channels[SM::RC_ARM_CHANNEL] > 90 &&
        system_manager->rc_data.rx_channels[SM::RC_THROTTLE_CHANNEL] == 0 &&
        system_manager->rc_data.rx_channels[SM::RC_PITCH_CHANNEL]) {
        system_manager->setState(FlightMode::getInstance());
    }
}

void GroundOpMode::exit(SystemManager *system_manager) {
    // Delete SM to AM queue to prevent PID tuning midair (not yet in Nov 27th ver)
    // osMessageQueueReset(system_manager->SM_to_AM_queue);
    // system_manager->SM_to_AM_queue = NULL;
}

SystemState &GroundOpMode::getInstance() {
    static GroundOpMode singleton;
    return singleton;
}

/********************
 * Flight Mode
 ********************/
void FlightMode::enter(SystemManager *system_manager) {
    // Start the PM thread and increase TM speed (have to start different task)
    // xTaskCreate(system_manager->PMOperationTask, "PM Thread", &system_manager->PM_handle);
    // vTaskDelete(system_manager->TM_handle);
    // xTaskCreate(system_manager->TMOperationTask, "TM Thread", &system_manager->TM_handle);

    // system_manager->SM_to_PM_queue = osMessageQueueNew(1, 256, NULL);
    // system_manager->PM_to_AM_queue = osMessageQueueNew(1, 256, NULL);

    // JUST for the Nov 27th deadline flight:

    system_manager->SM_to_AM_queue = osMessageQueueNew(1U, 256U, NULL);
    xTaskCreate(system_manager->AMOperationTask, "AM Thread", 400U,
                (void *)system_manager->attitude_manager, osPriorityNormal,
                &system_manager->AM_handle);
    // Task will automatically be added to the FreeRTOS scheduler
}

void FlightMode::execute(SystemManager *system_manager) {
    // Get sensor fusion data from LOS interfaces
    // TODO
    // SM.sf_data = los_interface.

    // Get RC data from LOS
    system_manager->rc_data = Los_Link::getInstance().getRx();
    if (system_manager->rc_data.rx_channels[SM::RC_ARM_CHANNEL] < 10) {
        system_manager->setState(DisarmMode::getInstance());
    }

    // Decode RC data and convert to AM message type
    system_manager->to_am_data = RcToAmInput(system_manager->rc_data);

    // Send to AM mail queue (JUST Nov 27th implementation)
    osMessageQueuePut(system_manager->SM_to_AM_queue, &system_manager->to_am_data)

    // Get big packet message from TM and store it
    // TODO

    // Determine flight state (Takeoff/cruise/Landing/landed) and update state
    // TODO
    // if ((system_manager->operation_mode == SM::LANDING && landedCheck()) ||
    // system_manager->tm_packet.state == "SM::DISARMED") {
    //     system_manager->operation_mode = SM::DISARMED;
    // }

    // // TODO update this when I get what TM sends/have a landing check
    // if (system_manager->tm_packet.desired_state == "FATAL_FAIL") {
    //     // If fatal Failure then move to fatal fail mode
    //     system_manager->setState(FatalFailMode::getInstance());
    // } else if (system_manager->operation_mode == SM::DISARMED) {
    //     // If disarm/landed then move to disarm
    //     system_manager->setState(DisarmMode::getInstance());
    // }

    // Get waypoint(s) from TM/RC (through LOS) and pass SF data and waypoint(s)
    // to PM (who sends to AM)
    // TODO

    // Get latest message back from AM
    // TODO

    // Create to-ground packet with AM response and SF and send through mail queue to TM
    // TODO
}

void FlightMode::exit(SystemManager *system_manager) {
    // Stop the AM, PM thread and decrease TM speed

    // Stop PM
    // vTaskDelete(system_manager->PM_handle);
    // Stop AM
    vTaskDelete(system_manager->AM_handle);

    // Slow TM operation speed
    // vTaskDelete(system_manager->TM_handle);
    // xTaskCreate(system_manager->TMSlowTask, "TM Slow Thread", &system_manager->TM_handle);

    osMessageQueueReset(system_manager->SM_to_AM_queue);
    system_manager->SM_to_AM_queue = NULL;

    // osMessageQueueReset(system_manager->SM_to_PM_queue);
    // system_manager->SM_to_PM_queue = NULL;
    // osMessageQueueReset(system_manager->PM_to_AM_queue);
    // system_manager->PM_to_AM_queue = NULL;

    // Write 0 to LOS actuators
    for (uint8_t channel; channel < MAX_PPM_CHANNELS; channel++) {
        Los_Actuators::getInstance().set(channel, 0);
    }
}

AttitudeManagerInput FlightMode::RcToAmInput(LosLinkRx_t rc_message) {
    AttitudeManagerInput am_message;

    // Get channel values from RC message
    // map pitch/roll/yaw to +/- 30 degrees
    // leave throttle between 0 and 100
    float throttle = rc_message.rx_channels[SM::RC_THROTTLE_CHANNEL] / 20.0;
    // So pulling down on stick goes up
    float pitch = (rc_message.rx_channels[SM::RC_PITCH_CHANNEL] * 60.0 / 100.0) - 30.0;
    // RHR with thumb pointed in direction of flight. CW looking forward = positive
    float roll = (rc_message.rx_channels[SM::RC_ROLL_CHANNEL] * 60.0 / 100.0) - 30.0;
    float yaw = (rc_message.rx_channels[SM::RC_YAW_CHANNEL] * 60.0 / 100.0) - 30.0;

    // map RC values into waypoint, assuming 50 for pitch, roll, and yaw means no change desired.
    // sets the maximum
    am_message.x_dir = cos(pitch) * cos(roll);
    // Positive right
    am_message.y_dir = sin(roll);
    // Positive down, (so pulling controller stick down moves drone up)
    am_message.z_dir = sin(pitch);
    // Note that the above is allready normalized
    am_message.magnitude = throttle;
    am_message.heading = yaw;

    return am_message;
}

SystemState &FlightMode::getInstance() {
    static FlightMode singleton;
    return singleton;
}

// CommandsFromSM* FlightMode::generatePmPacket(SystemManager *system_manager) {
//     // Pack SF and Waypoint data to packet sent to PM
//     CommandsFromSM pm_packet;

//     pm_packet = system_manager->sf_data;

//     if (system_manager->RC_new_data) { // TODO make this match what we get
//         system_manager->AM_Input = getManualWaypoint(system_manager->RC_msg);
//     }

//     //TODO
//     return &pm_packet;
// }

/********************
 * FatalFailure Mode
 ********************/

void FatalFailureMode::enter(SystemManager *system_manager) {
    // Stop PM
    // vTaskDelete(system_manager->PM_handle); Not yetrunning for Nov 27th.
    // Stop AM
    vTaskDelete(system_manager->AM_handle);

    // For Nov 27th just free the SM to AM queue
    osMessageQueueReset(system_manager->SM_to_AM_queue);
    system_manager->SM_to_AM_queue = NULL;

    // osMessageQueueReset(system_manager->SM_to_PM_queue);
    // system_manager->SM_to_PM_queue = NULL;
    // osMessageQueueReset(system_manager->PM_to_AM_queue);
    // system_manager->PM_to_AM_queue = NULL;
    // osMessageQueueReset(system_manager->PM_to_AM_queue);
    // system_manager->AM_to_SM_queue = NULL;

    // Write 0 to LOS actuators
    for (uint8_t channel; channel < MAX_PPM_CHANNELS; channel++) {
        Los_Actuators::getInstance().set(channel, 0);
    }
}

void FatalFailureMode::execute(SystemManager *system_manager) {
    // Maybe report something? Log timstamps to SD card?
    // TBD
    system_manager->setState(FatalFailureMode::getInstance());
}

SystemState &FatalFailureMode::getInstance() {
    static FatalFailureMode singleton;
    return singleton;
}

}  // namespace SM
