/*
 * SM_States.cpp
 *
 * System Manager State-Machine classes
 *
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 */

#include "../Inc/SM_States.hpp"

/********************
 * Boot Mode
 ********************/
void BootMode::execute(SystemManager *system_manager) {
    // Only Executes once, Configure sensors, LOS, Everything startup

    system_manager->los_interface.init(losInitData); // TODO, make this agree with what LOS needs
    
    // Start TM at slow speed
    // Update TM mail queue ID
    // system_manager->TM_to_SM_queue = osMailCreate();
    // xTaskCreate(system_manager->TMSlowTask, "TM Slow Thread", &system_manager->TM_handle);
    // vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked

    // AM init
    AM::ActuatorConfig m0{.channel = 0,
                          .stateMix = AM::StateMix(0, 0, 1, 1, 1, -1)};
    AM::ActuatorConfig m1{.channel = 1,
                          .stateMix = AM::StateMix(0, 0, 1, -1, -1, -1)};
    AM::ActuatorConfig m2{.channel = 2,
                          .stateMix = AM::StateMix(0, 0, 1, 1, -1, 1)};
    AM::ActuatorConfig m3{.channel = 3,
                          .stateMix = AM::StateMix(0, 0, 1, -1, 1, 1)};
    AM::LevelQuadControl quad1(m0, m1, m2, m3);

    system_manager->attitude_manager = attitude_manager(&quad1)

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
    system_manager->rc_data = system_manager->los_interface.los_link.getRx();
    if (system_manager->rc_data[SM::RC_ARM_CHANNEL] == 100) {
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
    system_manager->rc_data = system_manager->los_interface.los_link.getRx();
    if (system_manager->rc_data[SM::RC_ARM_CHANNEL] == 100 && system_manager->rc_data[SM::RC_THROTTLE_CHANNEL] == 100) {
        system_manager->setState(GroundOpMode::getInstance());
    }
}

void GroundOpMode::exit(SystemManager *system_manager) {
    // Delete SM to AM queue to prevent PID tuning midair
    osMailFree(system_manager->SM_to_AM_queue);
    system_manager->SM_to_AM_queue = NULL;
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

    // system_manager->SM_to_PM_queue = osMailCreate();
    // system_manager->PM_to_AM_queue = osMailCreate();

    // JUST for the Nov 27th deadline flight:

    system_manager->SM_to_AM_queue = osMailCreate();
    xTaskCreate(system_manager->AMOperationTask, "AM Thread", &system_manager->AM_handle);

    vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked
}

void FlightMode::execute(SystemManager *system_manager) {
    // Get sensor fusion data from LOS interfaces
    // TODO
    // SM.sf_data = los_interface.

    // Get RC data from LOS
    system_manager->rc_data = system_manager->los_interface.los_link.getRx();

    // Decode RC data and convert to AM message type
    system_manager->to_am_data = RcToAmInput(system_manager->rc_data);

    // Send to AM mail queue (JUST Nov 27th implementation)
    osMailPut(system_manager->SM_to_AM_queue, &system_manager->to_am_data)

    // Get big packet message from TM and store it
    // TODO

    // Determine flight state (Takeoff/cruise/Landing/landed) and update state
    // TODO
    // if ((system_manager->operation_mode == SM::LANDING && landedCheck()) || system_manager->tm_packet.state == "SM::DISARMED") {
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
    
    // Get waypoint(s) from TM/RC (through LOS) and pass SF data and waypoint(s) to PM (who sends to AM)
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

    vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked

    osMailFree(system_manager->SM_to_AM_queue);
    system_manager->SM_to_AM_queue = NULL;

    // osMailFree(system_manager->SM_to_PM_queue);
    // system_manager->SM_to_PM_queue = NULL;
    // osMailFree(system_manager->PM_to_AM_queue);
    // system_manager->PM_to_AM_queue = NULL;

    // Write 0 to LOS actuators
    // TODO
}

SystemState& FlightMode::getInstance() {
    static FlightMode singleton;
    return singleton;
}

inputs_to_AM_t* FlightMode::getManualWaypoint() {
    // Convert manual pilot controls from RC to AM waypoint
    // TODO
    return 0;
}

CommandsFromSM* FlightMode::generatePmPacket(SystemManager *system_manager) {
    // Pack SF and Waypoint data to packet sent to PM
    CommandsFromSM pm_packet;

    pm_packet = system_manager->sf_data;

    if (system_manager->RC_new_data) { // TODO make this match what we get
        system_manager->AM_Waypoints = getManualWaypoint(system_manager->RC_msg);
    } 

    //TODO
    return &pm_packet;
}


/********************
 * FatalFailure Mode
 ********************/

void FatalFailureMode::enter(SystemManager *system_manager) {
    // Stop PM
    vTaskDelete(system_manager->PM_handle);
    // Stop AM
    vTaskDelete(system_manager->AM_handle);

    vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked

    osMailFree(system_manager->SM_to_PM_queue);
    system_manager->SM_to_PM_queue = NULL;
    osMailFree(system_manager->PM_to_AM_queue);
    system_manager->PM_to_AM_queue = NULL;
    osMailFree(system_manager->PM_to_AM_queue);
    system_manager->AM_to_SM_queue = NULL;

    // Write 0 to LOS actuators
    // TODO
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
