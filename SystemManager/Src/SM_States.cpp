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
    // Only Executes once
    // TODO
}

/********************
 * Read Telemetry Mode
 ********************/
void ReadTelemetryMode::execute(SystemManager *sys_man) {
    // TODO
}

SystemState& ReadTelemetryMode::getInstance() {
    static ReadTelemetryMode singleton;
    return singleton;
}

// bool ReadTelemetryMode::receiveArmDisarmInstruction(SystemManager* sys_man) {
//     // if sm dc; return false
//     // if connected, return arm channel value
// }

// bool DisarmMode::isArmed() {
//     // check arm channel value > min arm value

// }


/********************
 * Read Los Sensors Mode
 ********************/
void ReadLosSensorsMode::execute(SystemManager *sys_man) {
    // TODO
}

SystemState &ReadLosSensorsMode::getInstance() {
    static ReadLosSensorsMode singleton;
    return singleton;
}

// bool ReadLosSensorsMode::receiveTeleopInstructions(SystemManager *sys_man) {
//     bool is_dc{true};
//     if (is_dc) {
//         return false;
//     }

//     // !TODO: get information from the link
//     // (and somehow process it)
// }

/********************
 * Generate Flight Plan Mode
 ********************/

void GenerateFlightPlanMode::execute(SystemManager *sys_man) {
    // Reading from bulk message from Telemetry
    if (telemetryMsg.manual_control) {
        AM_Waypoints = throttleToWaypoint();
    } else {
        AM_Waypoints = getPathManagerWaypoint();
    }
}

inputs_to_AM_t* GenerateFlightPlanMode::throttleToWaypoint() {
    // Handled by Aadi
    // Convert throttle inputs to waypoint that AM can read
    // telemetryMsg.throttle;
    return 0;
}

inputs_to_AM_t* GenerateFlightPlanMode::getPathManagerWaypoint() {
    // call PM for next waypoint? Also feed in SF data?
    return 0;
}

/********************
 * FatalFailure Mode
 ********************/

void FatalFailureMode::execute(SystemManager *sys_man) {
    sys_man->setState(FatalFailureMode::getInstance());
}

SystemState &FatalFailureMode::getInstance() {
    static FatalFailureMode singleton;
    return singleton;
}
