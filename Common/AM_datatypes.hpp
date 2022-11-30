#pragma once

struct AM_SM_Commands {
};

struct SM_AM_Commands {
    uint8_t motor_outputs[12];
};

struct PIDValues {
    double P;
    double I;
    double D;
};

struct PIDControllerValues {
    PIDValues axes[6];
};

struct GroundStationPIDSetCommand {
    uint8_t controller;
    uint8_t axis;
    PIDValues values;
};

struct GroundStationPIDSetResponse {
    uint8_t controller_number;
    PIDControllerValues controller;
};