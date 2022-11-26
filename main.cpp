#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Actuators.hpp"

void StartBlinkyTest(void * argument);

int main()
{
    losInit();

    osThreadAttr_t blinkyTest = {
        .name = "start_blinky",
        .stack_size = 128,
        .priority = osPriorityNormal
    };

    osThreadNew (StartBlinkyTest, NULL, &blinkyTest);

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