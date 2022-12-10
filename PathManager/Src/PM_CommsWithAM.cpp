/**
 * Defines functions for sending data from the path manager to the attitude manager.
 * Has function definitions used by both the attitude and path manager.
 * Author: Anthony Bertnyk
 */

//#include "CommWithAttitudeManager.hpp"
//#include "CommWithPathManager.hpp"
#include "PM_CommsWithAM.hpp"


extern "C"
{
#include "cmsis_os.h"
}

const char MAIL_Q_SIZE = 1;

//Set up a mail queue for sending commands to the attitude manager
osMailQDef(commandsMailQ, MAIL_Q_SIZE, PM_AM_Commands);
osMailQId commandsMailQID;


void CommFromPMToAMInit()
{
    /*TODO: THIS LINE NO WORK?*/
    commandsMailQID = osMailCreate(osMailQ(commandsMailQ), NULL);
}

void SendFromPMToAM(PM_AM_Commands *commands)
{
    //Remove previous command from mail queue if it exists
    osEvent event = osMailGet(commandsMailQID, 0);
    if(event.status == osEventMail)
    {
        osMailFree(commandsMailQID, static_cast<PM_AM_Commands *>(event.value.p));
    }

    //Allocate mail slot
    PM_AM_Commands *commandsOut;
    commandsOut = static_cast<PM_AM_Commands *>(osMailAlloc(commandsMailQID, osWaitForever));
    
    //Fill mail slot with data
    *commandsOut = *commands;

    //Post mail slot to mail queue
    osStatus sendStatus = osMailPut(commandsMailQID, commandsOut);
}


