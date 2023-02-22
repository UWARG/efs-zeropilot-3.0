#include "main.h"

#include "FreeRTOS.h"
#include "LOS_Actuators.hpp"
#include "SM.hpp"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_D_PPMChannel.hpp"
#include "LOS_D_PPMChannelOut.hpp"

void SMOperationTask(void *pvParameters);
const static auto SM_PERIOD_MS = 5;

int main() {
    losInit();

    TaskHandle_t SM_handle = NULL;

    xTaskCreate(SMOperationTask, "SM Thread", 400U, NULL, osPriorityNormal, &SM_handle);

    losKernelStart();

    // should not get here bec losInit() starts the scheduler
    while (1) {
    }

    // :monkey: :stabley2:
    PPM_Test();

    return 0;
}

void SMOperationTask(void *pvParameters) {
    SM::SystemManager SM_instance;

    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        SM_instance.execute();
        vTaskDelayUntil(&xNextWakeTime, SM_PERIOD_MS);
    }
}

void PPM_Test() {
	LosLinkRx_t losRxData;
	lostRxData.rx_channels = {10,01,49,02,14,0,520};
	Los_Link los_link();
	los_link.sendTx(0, losRxData);
}
