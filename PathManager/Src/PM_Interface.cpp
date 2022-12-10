#include "PM_Interface.h"
#include "PM.hpp"

static PathManager pathMng;
PathMan::Path_Manager_Cycle_Status pathManStatus;


bool PathManagerInterfaceExecute(void) {

    do {
            pathMng.execute();
            pathManStatus = pathMng.getStatus();

            if (pathManStatus == PathMan::FAILURE_MODE)
            {
                // Something is quite wrong, need to switch over to safety
                return false;
            }

    } while(pathManStatus != PathMan::COMPLETED_CYCLE);
    return true;
}
