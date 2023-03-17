#include "main.h"

#include "FreeRTOS.h"
#include "LOS_Actuators.hpp"
#include "SM.hpp"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Link.hpp"

void PPM_Test();
void SMOperationTask(void *pvParameters);
const static auto SM_PERIOD_MS = 5;

int main() {

	//put interrupt
    losInit();

    TaskHandle_t SM_handle = NULL;

    // :monkey: :stabley2:
    //PPM_Test();

    //xTask(SMOperationTask, "SM Thread", 400U, NULL, osPriorityNormal, &SM_handle);

    //losKernelStart();


    // should not get here bec losInit() starts the scheduler

    Los_Link* los_link = &Los_Link::getInstance();
    LosLinkTx_t losTxData = {};
    LosLinkRx_t losRxData = {};
    GPIO_PinState blink_value = GPIO_PIN_RESET;
    while (1) {
        // Receive
        losRxData = los_link->getRx(0);
        // Copy receive data to send data
        for (uint8_t i = 0; i < 4; ++i) {
            losTxData.tx_channels[i] = losRxData.rx_channels[i];
        }
        // Send
        los_link->sendTx(0, losTxData);
        // Blink test
        blink_value = (losRxData.rx_channels[0] > 0.5f) ? GPIO_PIN_SET : GPIO_PIN_RESET;
        HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, blink_value);
        // Small delay
        HAL_Delay(50);
    }


//    while (1) {
//
//    	TIM4->CNT;
//
//
//    	PPM_Test();
//    }





    return 0;
}

void SMOperationTask(void *pvParameters) {

    SM::SystemManager SM_instance;

    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
        SM_instance.execute();
        // :monkey: :stabley2:

        vTaskDelayUntil(&xNextWakeTime, SM_PERIOD_MS);
    }
}

void PPM_Test() {
	LosLinkTx_t losTxData = {};
	losTxData.tx_channels[0] = 0.1f;
    losTxData.tx_channels[1] = 0.01f;
    losTxData.tx_channels[2] = 0.49f;
    losTxData.tx_channels[3] = 1.f;
	Los_Link* los_link = &Los_Link::getInstance();
	los_link->sendTx(0, losTxData);
}
