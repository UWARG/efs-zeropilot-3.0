/**
 * Communicates with the attitude manager to send commands and receive useful data.
 * All functions here should be called by the path manager.
 * Author: Anthony Bertnyk
 */

#ifndef PM_COMM_WITH_AM_HPP
#define PM_COMM_WITH_AM_HPP

//#include "AttitudePathInterface.hpp"
#include "PM_DataTypes.hpp"


/**
 * Initiate sending of data to the attitude manager. Called once by path manager.
 */
void CommFromPMToAMInit();

/**
 * Send commands to the attitude manager. Called by path manager. CommFromPMToAMInit must be called once before this function can be called.
 * @param[in]   commands   Pointer to struct with commands.
 */
void SendFromPMToAM(PM_AM_Commands *commands);

/**
 * Retrieve new attitude/airspeed data from attitude manager if available. Called by path manager at any time.
 * @param[out]   data   Pointer to struct to recieve attitude/airspeed data.
 * @return True if data was recieved.
 */

#endif
