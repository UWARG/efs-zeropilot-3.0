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
void BootMode::enter(SystemManager *sys_man) {
    // Only Executes once, Configure sensors, LOS, Everything startup
    // Start TM at slow speed

    // Update TM mail queue ID
    TM_to_SM_queue = osMailCreate();
}

void BootMode::execute(SystemManager *sys_man) {
    // Immediately move to Disarm mode
    sys_man->setState(DisarmMode::getInstance());
}

SystemState &BootMode::getInstance() {
    static BootMode singleton;
    return singleton;
}

/********************
 * Disarm Mode
 ********************/
void DisarmMode::enter(SystemManager *sys_man) {
    // Start AM to accept PID tunings
    // TODO
    SM_to_AM_queue = osMailCreate();
}

void DisarmMode::execute(SystemManager *sys_man) {
    // Forward any AM PID tuning messages.
    // TODO
    // Check all arming mechnisms triggered then move to GroundOp
    // TODO
    sys_man->setState(GroundOpMode::getInstance());
}

SystemState &DisarmMode::getInstance() {
    static DisarmMode singleton;
    return singleton;
}

/********************
 * Ground Op Mode
 ********************/
void GroundOpMode::execute(SystemManager *sys_man) {
    
    sys_man->setState(FlightMode::getInstance());
}

void GroundOpMode::exit(SystemManager *sys_man) {
    // Delete SM to AM queue to prevent PID tuning midair
    osMailFree(SM_to_AM_queue);
    SM_to_AM_queue = NULL;
}

SystemState &GroundOpMode::getInstance() {
    static GroundOpMode singleton;
    return singleton;
}

/********************
 * Flight Mode
 ********************/
void FlightMode::enter(SystemManager *sys_man) {
    // Start the PM thread and increase TM speed
    // TODO
    SM_to_PM_queue = osMailCreate();
    PM_to_AM_queue = osMailCreate();
}

void FlightMode::execute(SystemManager *sys_man) {
    // Get big packet message from TM and store it
    // TODO

    // Determine flight state (Takeoff/cruise/Landing) and update state
    // TODO

    // If disarm then move to disarm
    // sys_man->setState(DisarmMode::getInstance());

    // If fatal Failure then move to disarm
    // sys_man->setState(FatalFailMode::getInstance());

    // Get sensor fusion data from LOS interfaces
    // TODO
    
    // Get waypoint(s) from TM/RC (through LOS) and pass SF data and waypoint(s) to PM (who sends to AM)
    // TODO

    // Get latest message back from AM
    // TODO

    // Create to-ground packet with AM response and SF and send through mail queue to TM
    // TODO
}

void FlightMode::exit(SystemManager *sys_man) {
    // Stop the PM thread and decrease TM speed
    osMailFree(SM_to_PM_queue);
    SM_to_PM_queue = NULL;
    osMailFree(PM_to_AM_queue);
    PM_to_AM_queue = NULL;
    // TODO
}

SystemState& FlightMode::getInstance() {
    static FlightMode singleton;
    return singleton;
}

inputs_to_AM_t* FlightMode::generateWaypoint() {
    // Generate a waypoint to be sent to PM
    if (telemetryMsg.manual_control) {
        AM_Waypoints = getManualWaypoint();
    } else {
        AM_Waypoints = getPathManagerWaypoint();
    }
    return AM_Waypoints;
}

inputs_to_AM_t* FlightMode::getManualWaypoint() {
    // Convert manual pilot controls to AM waypoint
    // TODO
    return 0;
}

inputs_to_AM_t* FlightMode::getPathManagerWaypoint() {
    // Create the waypoint(s) to pass to path manager
    // TODO
    return 0;
}

CommandsFromSM* FlightMode::generatePMPacket() {
    // Pack SF and Waypoint data to packet sent to PM
    //TODO
    return 0;
}


/********************
 * FatalFailure Mode
 ********************/

void FatalFailureMode::enter(SystemManager *sys_man) {
    // Kill PM
    // Kill AM

    osMailFree(SM_to_PM_queue);
    SM_to_PM_queue = NULL;
    osMailFree(PM_to_AM_queue);
    PM_to_AM_queue = NULL;
    osMailFree(PM_to_AM_queue);
    AM_to_SM_queue = NULL;

    // Write 0 to LOS actuators
    // TODO
}

void FatalFailureMode::execute(SystemManager *sys_man) {
    // Maybe report something? Log timstamps to SD card?
    // TBD
    sys_man->setState(FatalFailureMode::getInstance());
}

SystemState &FatalFailureMode::getInstance() {
    static FatalFailureMode singleton;
    return singleton;
}
