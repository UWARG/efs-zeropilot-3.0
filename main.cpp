#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "cmsis_os.h"
#include "LOS_Actuators.hpp"

osThreadId blinkyTestHandle;
osThreadId losActuatorsTestHandle;

void StartBlinkyTest(void const * argument);
void StartLosActuatorsTest(void const * argument);


int main()
{
    /* Init scheduler */
    osKernelInitialize();

    osThreadDef(blinkyTest, StartBlinkyTest, osPriorityNormal, 0, 128);
    osThreadDef(losActuatorsTest, StartLosActuatorsTest, osPriorityNormal, 0, 1024);
    blinkyTestHandle = osThreadCreate(osThread(blinkyTest), NULL);
    losActuatorsTestHandle = osThreadCreate(osThread(losActuatorsTest), NULL);

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

void StartLosActuatorsTest(void const * argument)
{
    for (;;)
    {
        Los_Actuators::getInstance().set(0, 50);
        osDelay(2000);
        Los_Actuators::getInstance().set(0, 100);
        osDelay(2000);
        Los_Actuators::getInstance().set(0, 0);
        osDelay(2000);
    }

}