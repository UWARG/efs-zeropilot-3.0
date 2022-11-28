#pragma once

#include "PM_datatypes.hpp"

// goes from TM to SM
struct GroundStationDisarm {
    bool arm;
};

// goes from SM to TM
struct GroundStationData {
    uint8_t motor_outputs[12];
    LosSFData sf_data;
    uint8_t battery_voltages[13];
    uint8_t controller_values[16];
};