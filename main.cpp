#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Actuators.hpp"

#include "SM.hpp"

void SMOperationTask(void *pvParameters);
const static auto SM_PERIOD_OPERATION_MS = 5;

SystemManager SM_instance;

int main()
{
    losInit();

    TaskHandle_t SM_handle = NULL;

    xTaskCreate(SMOperationTask, "SM Thread", 400, NULL, osPriorityNormal, &SM_handle);

    losKernelStart();

    //should not get here bec losInit() starts the scheduler
    while(1){}

    return 0;
}

void SMOperationTask(void *pvParameters)
{
    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        SM_instance.execute();
        vTaskDelayUntil(&xNextWakeTime, SM_PERIOD_OPERATION_MS);
    }
}
