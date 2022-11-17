/**
 * Getting data from SM 
 */

#ifndef PM_COMMS_WITH_SM_HPP
#define PM_COMMS_WITH_SM_HPP

#include "PM_DataTypes.hpp"

/**
 * Initiate communication with telemetry.
 */
void CommWithSMInit();

/**
 * Retrieve commands from SM.
 * @param[out]   commands   Pointer to struct to recieve commands.
 * @return True if commands were recieved.
 */
bool GetSMCommands(CommandsFromSM *commands);

#endif
