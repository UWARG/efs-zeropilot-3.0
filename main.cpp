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

    Los_Link* los_link = &Los_Link::getInstance();
    LosLinkTx_t losTxData = {};
    LosLinkRx_t losRxData = {};
    GPIO_PinState blink_value = GPIO_PIN_RESET;
    while (1) {
        // Receive
        losRxData = los_link->getRx(0);
        // Copy receive data to send data
        for (uint8_t i = 0; i < 8; ++i) {
            losTxData.tx_channels[i] = losRxData.rx_channels[i];
        }
        // Send
        los_link->sendTx(0, losTxData);
        // Blink test
        blink_value = (losRxData.rx_channels[0] > 0.50f) ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, blink_value);
        // Small delay
        HAL_Delay(50);
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
