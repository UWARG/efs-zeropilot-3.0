#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

osThreadId blinkyTestHandle;

void StartBlinkyTest(void const * argument);


int main()
{

    osThreadDef(blinkyTest, StartBlinkyTest, osPriorityNormal, 0, 128);
    blinkyTestHandle = osThreadCreate(osThread(blinkyTest), NULL);

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
