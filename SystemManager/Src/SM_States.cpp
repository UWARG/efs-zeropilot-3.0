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
void BootMode::execute(SystemManager *sys_man) {
    // Only Executes once, Configure sensors, LOS, Everything startup

    los_interface.init(losInitData); // TODO, make this agree with what LOS needs

    // Start TM at slow speed
    xTaskCreate(sys_man.TMSlowTask, "TM Slow Thread", &sys_man.TM_handle);
    
    vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked

    // Update TM mail queue ID
    sys_man.TM_to_SM_queue = osMailCreate();

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
    xTaskCreate(sys_man.AMOperationTask, "AM Thread", &sys_man.AM_handle);

    vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked

    sys_man.SM_to_AM_queue = osMailCreate();
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
    osMailFree(sys_man.SM_to_AM_queue);
    sys_man.SM_to_AM_queue = NULL;
}

SystemState &GroundOpMode::getInstance() {
    static GroundOpMode singleton;
    return singleton;
}

/********************
 * Flight Mode
 ********************/
void FlightMode::enter(SystemManager *sys_man) {
    // Start the PM thread and increase TM speed (have to start different task)
    xTaskCreate(sys_man.PMOperationTask, "PM Thread", &sys_man.PM_handle);
    vTaskDelete(sys_man.TM_handle);
    xTaskCreate(sys_man.TMOperationTask, "TM Thread", &sys_man.TM_handle);

    sys_man.SM_to_PM_queue = osMailCreate();
    sys_man.PM_to_AM_queue = osMailCreate();

    vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked
}

void FlightMode::execute(SystemManager *sys_man) {
    // Get sensor fusion data from LOS interfaces
    // TODO
    // SM.sf_data = los_interface.

    // Get big packet message from TM and store it
    // TODO

    // Determine flight state (Takeoff/cruise/Landing/landed) and update state
    // TODO

    // If disarm/landed then move to disarm
    // sys_man->setState(DisarmMode::getInstance());

    // If fatal Failure then move to fatal fail
    // sys_man->setState(FatalFailMode::getInstance());
    
    // Get waypoint(s) from TM/RC (through LOS) and pass SF data and waypoint(s) to PM (who sends to AM)
    // TODO

    // Get latest message back from AM
    // TODO

    // Create to-ground packet with AM response and SF and send through mail queue to TM
    // TODO

}

void FlightMode::exit(SystemManager *sys_man) {
    // Stop the AM, PM thread and decrease TM speed

    // Kill PM
    vTaskDelete(sys_man.PM_handle);
    // Kill AM
    vTaskDelete(sys_man.AM_handle);

    // Slow TM operation speed
    vTaskDelete(sys_man.TM_handle);
    xTaskCreate(sys_man.TMSlowTask, "TM Slow Thread", &sys_man.TM_handle);

    vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked

    osMailFree(sys_man.SM_to_PM_queue);
    sys_man.SM_to_PM_queue = NULL;
    osMailFree(sys_man.PM_to_AM_queue);
    sys_man.PM_to_AM_queue = NULL;

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

CommandsFromSM* FlightMode::generatePMPacket(SystemManager *sys_man) {
    // Pack SF and Waypoint data to packet sent to PM
    CommandsFromSM pm_packet;

    pm_packet = sys_man.sf_data;

    if (sys_man.RC_new_data) { // TODO make this match what we get
        sys_man.AM_Waypoints = getManualWaypoint(sys_man.RC_msg);
    } 
    
    

    //TODO
    return 0;
}


/********************
 * FatalFailure Mode
 ********************/

void FatalFailureMode::enter(SystemManager *sys_man) {
    // Kill PM
    vTaskDelete(sys_man.PM_handle);
    // Kill AM
    vTaskDelete(sys_man.AM_handle);

    vTaskStartScheduler(); // TODO, make sure I'm using this right, haven't checked
    

    osMailFree(sys_man.SM_to_PM_queue);
    sys_man.SM_to_PM_queue = NULL;
    osMailFree(sys_man.PM_to_AM_queue);
    sys_man.PM_to_AM_queue = NULL;
    osMailFree(sys_man.PM_to_AM_queue);
    sys_man.AM_to_SM_queue = NULL;

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
