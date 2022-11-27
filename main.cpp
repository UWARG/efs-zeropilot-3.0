#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Actuators.hpp"
#include "LOS_Link.hpp"

void StartBlinkyTest(void * argument);
void StartLosLinkTest(void * argument);

int main()
{
    losInit();

    osThreadAttr_t blinkyTest = {
        .name = "start_blinky",
        .stack_size = 128,
        .priority = osPriorityNormal
    };

    osThreadAttr_t losLinkTest = {
        .name = "start_los_link",
        .stack_size = 128,
        .priority = osPriorityNormal
    };

    osThreadNew (StartBlinkyTest, NULL, &blinkyTest);
    osThreadNew (StartLosLinkTest, NULL, &losLinkTest);

    losKernelStart();

    //should not get here bec losInit() starts the scheduler
    while(1){}

    return 0;
}


void StartBlinkyTest(void * argument)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(250);
        osDelay(1);
    }
}

void StartLosLinkTest(void * argument)
{
    for (;;)
    {
        LosLinkRx_t testVal = Los_Link::getInstance().getRx(0);
        osDelay(1);
    }

}
