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
void DisarmMode::execute(SystemManager *sys_man) {
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
void GroundOpMode::enter(SystemManager *sys_man) {
    // Start AM to accept PID tunings
    // TODO
    SM_to_AM_queue = osMailCreate();
}

void GroundOpMode::execute(SystemManager *sys_man) {
    // Forward any AM PID tuning messages.

    // TODO
    sys_man->setState(ReadTelemetryMode::getInstance());
}

void GroundOpMode::exit(SystemManager *sys_man) {
    // Delete SM to AM queue to prevent PID tuning midair
    osMailFree(SM_to_AM_queue);
    SM_to_AM_queue = NULL;

    // Start the PM thread and increase TM speed
    SM_to_PM_queue = osMailCreate();
    PM_to_AM_queue = osMailCreate();
    // TODO
}

SystemState &GroundOpMode::getInstance() {
    static GroundOpMode singleton;
    return singleton;
}

/********************
 * Read Telemetry Mode
 ********************/
void ReadTelemetryMode::execute(SystemManager *sys_man) {
    // Get big packet message from TM and store it
    // TODO
    sys_man->setState(ReadLosSensorsMode::getInstance());
}

SystemState& ReadTelemetryMode::getInstance() {
    static ReadTelemetryMode singleton;
    return singleton;
}


/********************
 * Read Los Sensors Mode
 ********************/
void ReadLosSensorsMode::execute(SystemManager *sys_man) {
    // Get sensor fusion data from LOS interfaces
    // TODO
    sys_man->setState(SendFlightPlanMode::getInstance());
}

SystemState &ReadLosSensorsMode::getInstance() {
    static ReadLosSensorsMode singleton;
    return singleton;
}

/********************
 * Send Flight Plan Mode
 ********************/

void SendFlightPlanMode::execute(SystemManager *sys_man) {
    // Get waypoint(s) and pass SF data and waypoint(s) to PM (who sends to AM)
    // TODO
    sys_man->setState(SendFlightPlanMode::getInstance());
}

inputs_to_AM_t* SendFlightPlanMode::generateWaypoint() {
    // Generate a waypoint to be sent to PM
    if (telemetryMsg.manual_control) {
        AM_Waypoints = getManualWaypoint();
    } else {
        AM_Waypoints = getPathManagerWaypoint();
    }
    return AM_Waypoints;
}

inputs_to_AM_t* SendFlightPlanMode::getManualWaypoint() {
    // Convert manual pilot controls to AM waypoint
    // TODO
    return 0;
}

inputs_to_AM_t* SendFlightPlanMode::getPathManagerWaypoint() {
    // Create the waypoint(s) to pass to path manager
    // TODO
    return 0;
}

CommandsFromSM* SendFlightPlanMode::generatePMPacket() {
    // Pack SF and Waypoint data to packet sent to PM
    //TODO
    return 0;
}

SystemState &SendFlightPlanMode::getInstance() {
    static SendFlightPlanMode singleton;
    return singleton;
}

/********************
 * Write Telemetry Mode
 ********************/
void WriteTelemetryMode::execute(SystemManager *sys_man) {
    // Create packet and send through mail queue to TM
    // TODO
    sys_man->setState(ReadTelemetryMode::getInstance());
}

SystemState& WriteTelemetryMode::getInstance() {
    static WriteTelemetryMode singleton;
    return singleton;
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
