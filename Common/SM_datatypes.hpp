#ifndef ZPSW3_SM_COMMON_DATATYPES_HPP
#define ZPSW3_SM_COMMON_DATATYPES_HPP

#include "PM_datatypes.hpp"
#include "AM_datatypes.hpp"
#include <stdint.h>

struct SM_TM_Commands {
    GroundStationData gspc_data;
    JetsonMovementRequest jetson_movement_req;
    GroundStationPIDSetResponse gspc_pid_set_resp;
};

struct TM_SM_Commands {
    GroundStationDisarm ground_station_disarm;
    WaypointsCommand waypoint_commands;
    JetsonToZpMovementCommand jetson_commands; 
    LandingInitiationCommand landing_initiation; 
    GroundStationPIDSetCommand gspc_pid_set_command;
};

struct GroundStationDisarm {
    bool arm;
};

struct GroundStationData {
    uint8_t motor_outputs[12];
    LosSFData sf_data;
    uint8_t battery_voltages[13];
    uint8_t controller_values[16];
};

#endif
