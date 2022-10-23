#pragma once
#include "PM_StateManager.hpp"
#include "PM_DataTypes.hpp"
#include "PM_LandingTakeoffManager.hpp"

class pathManagerState;

namespace PathMan
{

// Gives status of Path Manager so we know when it has completed a cycle, or entered failure mode.
enum Path_Manager_Cycle_Status {COMPLETED_CYCLE = 0, IN_CYCLE, FAILURE_MODE};

}

class pathManager
{
    public:
        pathManager();
        inline pathManagerState* getCurrentState() const {return currentState;}
        void execute();
        void setState(pathManagerState& newState);
        PathMan::Path_Manager_Cycle_Status getStatus() {return status;}

        //used to determine the stage of the landing sequence
        LandingTakeoffManager vtolManager;
        FlightStage flight_stage;
        bool isError;
       
    private:
        pathManagerState* currentState;
        PathMan::Path_Manager_Cycle_Status status;
};


