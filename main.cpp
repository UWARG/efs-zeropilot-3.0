#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "LOS_Link.hpp"
#include "interface_datatypes.hpp"

osThreadId blinkyTestHandle;
osThreadId losLinkTestHandle;

void StartBlinkyTest(void const * argument);
void StartLosLinkTest(void const * argument);


int main()
{

    osThreadDef(blinkyTest, StartBlinkyTest, osPriorityNormal, 0, 128);
    osThreadDef(losLinkTest, StartLosLinkTest, osPriorityNormal, 0, 128);

    blinkyTestHandle = osThreadCreate(osThread(blinkyTest), NULL);
    losLinkTestHandle = osThreadCreate(osThread(losLinkTest), NULL);

    losInit();

    //should not get here bec losInit() starts the scheduler
    while(1){}

    return 0;
}


void StartBlinkyTest(void const * argument)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(250);
        osDelay(1);
    }
}

void StartLosLinkTest(void const * argument)
{
    for (;;)
    {
        LosLinkRx_t testVal = Los_Link::getInstance().getRx();
        osDelay(1);
    }

}