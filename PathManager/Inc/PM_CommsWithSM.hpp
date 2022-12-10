/**
 * Getting data from SM 
 */

#ifndef ZPSW3_PM_COMMS_WITH_SM_HPP
#define ZPSW3_PM_COMMS_WITH_SM_HPP

#include "Common\Inc\PM_datatypes.hpp"
#include "PM_DataTypes.hpp"

/**
 * Initiate communication with SM.
 */
void CommWithSMInit();

/**
 * Retrieve commands from SM.
 * @param[out]   commands   Pointer to struct to recieve commands.
 * @return True if commands were recieved.
 */
bool GetSMCommands(SM_PM_Commands *commands);

#endif
