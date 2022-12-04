#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Actuators.hpp"

void StartBlinkyTest(void * argument);
void StartLosActuatorsTest(void * argument);


int main()
{
    /* Init scheduler */
    osKernelInitialize();

    osThreadAttr_t blinkyTest = {
        .name = "start_blinky",
        .stack_size = 512,
        .priority = osPriorityNormal
    };

    osThreadAttr_t losActTest = {
        .name = "los_act",
        .stack_size = 512,
        .priority = osPriorityNormal
    };

    osThreadNew (StartBlinkyTest, NULL, &blinkyTest);
    osThreadNew (StartLosActuatorsTest, NULL, &losActTest);

    losInit();
    losKernelStart();

    //should not get here bec losInit() starts the scheduler
    while(1){}

    return 0;
}


void StartBlinkyTest(void * argument)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
        osDelay(250);
    }
}

void StartLosActuatorsTest(void * argument)
{
    Los_Actuators actuators = Los_Actuators::getInstance();
    for (;;)
    {
        HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        actuators.set(0, 50);
        osDelay(2000);
        actuators.set(0, 0);
        osDelay(2000);
        actuators.set(0, 100);
        osDelay(2000);
    }

}
