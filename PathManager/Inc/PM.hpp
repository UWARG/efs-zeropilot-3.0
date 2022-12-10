#ifndef ZPSW3_PM_HPP
#define ZPSW3_PM_HPP

#include <stdint.h>
//#include "cmsis_os.h"

#include "PM_StateManager.hpp"
#include "Common\Inc\PM_datatypes.hpp"
#include "PM_DataTypes.hpp"
#include "PM_LandingTakeoffManager.hpp"

namespace PM
{
class PathManagerState;

// Gives status of Path Manager so we know when it has completed a cycle, or entered failure mode.
enum Path_Manager_Cycle_Status {COMPLETED_CYCLE = 0, IN_CYCLE, FAILURE_MODE};

class PathManager
{
    public:
        PathManager();
        inline PathManagerState* getCurrentState() const {return currentState;}
        void execute();
        void setState(PathManagerState& newState);
        Path_Manager_Cycle_Status getStatus() {return status;}

        //used to determine the stage of the landing sequence
        LandingTakeoffManager vtol_manager;
        FlightStage flight_stage;
        bool isError;

        void setSmQueue(osMessageQId queueId);
        osMessageQId getSmQueue();

        void setAmQueue(osMessageQId queueId);
        osMessageQId getAmQueue();



       
    private:
        PathManagerState* currentState;
        Path_Manager_Cycle_Status status;

         // Message Q
        osMessageQId SM_to_PM_queue;
        osMessageQId PM_to_AM_queue;
};

}



#endif

