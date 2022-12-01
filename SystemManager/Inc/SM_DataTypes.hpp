/*
 * SM_DataTypes.hpp
 *
 * System Manager Data Types Definition header
 *
 * Created on: Nov 7, 2022
 * Author(s): Gordon Fountain
 */

#ifndef ZPSW3_SM_DATATYPES_HPP
#define ZPSW3_SM_DATATYPES_HPP

// TODO remove this file once common datatypes are merged

// TODO Fill these out
typedef struct CommandsFromSM{
    WaypointType waypoint_type;
    TelemWaypointData telemetry_waypoint;
    CommandsFromTM telemetry_commands;
    LosSFData sf_data;
} CommandsFromSM;

// typedef struct {
//     int some_data_here;
// } Position_t;


// enum flight_mode: int8_t {
//     fm_fatal_failure = -1,
//     fm_limp = 0,
//     fm_stabilize = 1,
//     fm_gps = 2,
//     fm_autonomous = 3
// };

#endif //ZPSW3_SM_DATATYPES_HPP
