#pragma once



enum TM_Status {SUCCESS = 0, FAILURE};

class TelemetryManager {

    public:
        TelemetryManager();

        TM_Status execute();

        

};