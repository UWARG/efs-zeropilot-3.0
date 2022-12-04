

#include "PM_CommsWithSM.hpp"

extern "C"
{
#include "cmsis_os.h"
}
const char MAIL_Q_SIZE = 1;

osMailQDef(commandsMailQID, MAIL_Q_SIZE, SM_PM_Commands);
osMailQId commandsMailQID;

void CommWithSMInit()
{
    commandsMailQID = osMailCreate(osMailQ(commandsMailQ), NULL);


}

bool GetSMCommands(SM_PM_Commands *commands)
{
    osEvent event;
    SM_PM_Commands* commandsIn;
    event = osMailGet(commandsMailQID, 0);
    if(event.status == osEventMail)
    {
        commandsIn = static_cast<SM_PM_Commands *>(event.value.p);

        //Keep the command and remove it from the queue
        *commands = *commandsIn;
        osMailFree(commandsMailQID, commandsIn);
        return true;
    }
    else
    {
        //Indicate that no new commands are available.
        return false;
    }
    return true;
    
}
