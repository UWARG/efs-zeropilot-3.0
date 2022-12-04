

#include "PM_CommsWithSM.hpp"

extern "C"
{
#include "cmsis_os.h"
}
const char MAIL_Q_SIZE = 1;

osMailQDef(commandsMailQID, MAIL_Q_SIZE, CommandsFromSM);
osMailQId commandsMailQID;

void CommWithSMInit()
{
    commandsMailQID = osMailCreate(osMailQ(commandsMailQ), NULL);


}

bool GetSMCommands(CommandsFromSM *commands)
{
    osEvent event;
    CommandsFromSM* commandsIn;
    event = osMailGet(commandsMailQID, 0);
    if(event.status == osEventMail)
    {
        commandsIn = static_cast<CommandsFromSM *>(event.value.p);

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
