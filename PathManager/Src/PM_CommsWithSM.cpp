

#include "PM_CommsWithSM.hpp"

extern "C"
{
#include "cmsis_os.h"
}
const char PATH_TELEM_MAIL_Q_SIZE = 1;

osMailQDef(PIGOMailQ, PATH_TELEM_MAIL_Q_SIZE, fijo);
osMailQId PIGOMailQ;

void CommWithSMInit()
{
    PIGOMailQ = osMailCreate(osMailQ(PIGOMailQ), NULL);

}

bool GetSMCommands(CommandsFromSM *commands)
{
    osEvent event;
    CommandsFromSM* commandsIn;
    event = osMailGet(PIGOMailQ, 0);
    if(event.status == osEventMail)
    {
        commandsIn = static_cast<CommandsFromSM *>(event.value.p);

        //Keep the command and remove it from the queue
        *commands = *commandsIn;
        osMailFree(PIGOMailQ, commandsIn);
        return true;
    }
    else
    {
        //Indicate that no new commands are available.
        return false;
    }
    return true;
    
}
