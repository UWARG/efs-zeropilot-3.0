#pragma once

#include "CRC32.hpp"

enum TM_Status {SUCCESS = 0, FAILURE};

class TelemetryManager {


    uint32_t test;

    public:
        TelemetryManager();

        TM_Status execute();

        

};