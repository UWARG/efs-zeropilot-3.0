#pragma once

struct PIDValues {
    double P;
    double I;
    double D;
}

struct PIDControllerValues {
    PIDValues axes[6];
}

struct GroundStationPIDSetCommand {
    uint8_t controller;
    uint8_t axis;
    PIDValues values;
}

struct GroundStationPIDSetResponse {
    uint8_t controller_number;
    PIDControllerValues controller;
}