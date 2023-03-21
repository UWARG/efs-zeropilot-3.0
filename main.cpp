#include "main.h"

#include "FreeRTOS.h"
#include "LOS_Actuators.hpp"
#include "SM.hpp"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Link.hpp"

void SMOperationTask(void *pvParameters);
void PPM_Test();
const static auto SM_PERIOD_MS = 5;

int main() {
    losInit();

    PPM_Test();



    // should not get here bec losInit() starts the scheduler
    while (1) {

     HAL_Delay(100);
    }

    return 0;
}

void PPM_Test() {
    LosLinkTx_t losTxData = {};
    losTxData.tx_channels[0] = 0;
    losTxData.tx_channels[1] = 25;
    losTxData.tx_channels[2] = 75;
    losTxData.tx_channels[3] = 100;
    Los_Link* los_link = &Los_Link::getInstance();
    los_link->sendTx(0, losTxData);
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
