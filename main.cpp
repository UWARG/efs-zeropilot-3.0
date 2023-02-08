#include "main.h"

#include "FreeRTOS.h"
#include "LOS_Actuators.hpp"
#include "SM.hpp"
#include "TM.hpp"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Comms.hpp"
#include "usart.h"


void SMOperationTask(void *pvParameters);
const static auto SM_PERIOD_MS = 5;

int main() {
    losInit();

    TaskHandle_t SM_handle = NULL;

    xTaskCreate(SMOperationTask, "SM Thread", 400U, NULL, osPriorityNormal, &SM_handle);

    losKernelStart();
    //TelemetryManager tm;
   // HAL_UARTEx_ReceiveToIdle_DMA(&huart3, (uint8_t*) Los_Comms::getInstance().comms_buffer, sizeof(Los_Comms::getInstance().comms_buffer));
    //tm.init();

    // should not get here bec losInit() starts the scheduler
    while (1) {
      //  tm.receiveData();
      //  HAL_Delay(10);

    }

    return 0;
}

void SMOperationTask(void *pvParameters) {
    //SM::SystemManager SM_instance;
    TelemetryManager tm;
    tm.init();
    //Los_Comms::getInstance().init();


    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
       // SM_instance.execute();
       tm.receiveData();
       //vTaskDelayUntil(&xNextWakeTime, SM_PERIOD_MS);
     //   vTaskDelay(10);
    }
}
