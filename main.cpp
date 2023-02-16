#include "main.h"

#include "FreeRTOS.h"
// #include "LOS_Actuators.hpp"
// #include "SM.hpp"
#include "TM.hpp"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Comms.hpp"
#include "usart.h"


void SMOperationTask(void *pvParameters);
const static auto SM_PERIOD_MS = 10;

volatile uint8_t buffer[10];

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size) {
// 	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);

// 	if(huart->Instance == USART3) {
// 		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, (uint8_t *) buffer, sizeof(buffer));
// 		__HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);
// 	}
// }

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
//    //Los_Comms::getInstance().init();
//    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, (uint8_t*) buffer, 10);
//	__HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);

    TickType_t xNextWakeTime;
    xNextWakeTime = xTaskGetTickCount();
    while (true) {
       // SM_instance.execute();
      // tm.receiveData();
       vTaskDelayUntil(&xNextWakeTime, SM_PERIOD_MS);
     //   vTaskDelay(10);
    }
}
