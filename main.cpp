#include "main.h"

#include <cstring>

#include "FreeRTOS.h"
#include "LOS_D_DSHOTChannel.hpp"
#include "LOS_D_PWMChannel.hpp"
#include "SM.hpp"
#include "cmsis_os2.h"
#include "task.h"
#include "tim.h"
#include "usart.h"

void GPIOTask(void *pvParameters);
void LEDTask(void *pvParameters);
void UARTTask(void *pvParameters);
void SDMMCTask(void *pvParameters);
void PWMTask(void *pvParameters);

void SMOperationTask(void *pvParameters);
const static auto SM_PERIOD_MS = 5;

int main() {
    losInit();

    // TaskHandle_t SM_handle = NULL;
    // xTaskCreate(SMOperationTask, "SM Thread", 400U, NULL, osPriorityNormal, &SM_handle);

    TaskHandle_t hGPIO = NULL;
    xTaskCreate(GPIOTask, "GPIO", 50U, NULL, osPriorityNormal, &hGPIO);

    TaskHandle_t hLED = NULL;
    xTaskCreate(LEDTask, "LED", 50U, NULL, osPriorityNormal, &hLED);

    TaskHandle_t hUART = NULL;
    xTaskCreate(UARTTask, "UART", 500U, NULL, osPriorityNormal, &hUART);

    TaskHandle_t hSDMMC = NULL;
    xTaskCreate(SDMMCTask, "SDMMC", 100U, NULL, osPriorityNormal, &hSDMMC);

    TaskHandle_t hPWM = NULL;
    xTaskCreate(PWMTask, "PWM", 500U, NULL, osPriorityNormal, &hPWM);

    losKernelStart();

    // should not get here bec losInit() starts the scheduler
    while (1) {
    }

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

void GPIOTask(void *pvParameters) {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 2;

    GPIO_PinState Interlock_A = HAL_GPIO_ReadPin(GPIO_Interlock_A_GPIO_Port, GPIO_Interlock_A_Pin);
    GPIO_PinState Interlock_B = HAL_GPIO_ReadPin(GPIO_Interlock_B_GPIO_Port, GPIO_Interlock_B_Pin);
    GPIO_PinState Interlock_C = HAL_GPIO_ReadPin(GPIO_Interlock_C_GPIO_Port, GPIO_Interlock_C_Pin);
    GPIO_PinState Interlock_D = HAL_GPIO_ReadPin(GPIO_Interlock_D_GPIO_Port, GPIO_Interlock_D_Pin);

    if (!Interlock_A || !Interlock_B || !Interlock_C || !Interlock_D)
        while (true) {
        }

    while (true) {
        HAL_GPIO_TogglePin(GPIO_1_GPIO_Port, GPIO_1_Pin);
        HAL_GPIO_TogglePin(GPIO_2_GPIO_Port, GPIO_2_Pin);
        HAL_GPIO_TogglePin(GPIO_3_GPIO_Port, GPIO_3_Pin);
        HAL_GPIO_TogglePin(GPIO_4_GPIO_Port, GPIO_4_Pin);
        HAL_GPIO_TogglePin(GPIO_5_GPIO_Port, GPIO_5_Pin);
        HAL_GPIO_TogglePin(GPIO_6_GPIO_Port, GPIO_6_Pin);
        HAL_GPIO_TogglePin(GPIO_7_GPIO_Port, GPIO_7_Pin);
        HAL_GPIO_TogglePin(GPIO_8_GPIO_Port, GPIO_8_Pin);
        HAL_GPIO_TogglePin(GPIO_9_GPIO_Port, GPIO_9_Pin);
        HAL_GPIO_TogglePin(GPIO_10_GPIO_Port, GPIO_10_Pin);

        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}

void LEDTask(void *pvParameters) {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 2;

    while (true) {
        HAL_GPIO_TogglePin(GPIO_LED_1_YELLOW_GPIO_Port, GPIO_LED_1_YELLOW_Pin);
        HAL_GPIO_TogglePin(GPIO_LED_1_GREEN_GPIO_Port, GPIO_LED_1_GREEN_Pin);
        HAL_GPIO_TogglePin(GPIO_LED_1_RED_GPIO_Port, GPIO_LED_1_RED_Pin);

        HAL_GPIO_TogglePin(GPIO_LED_2_YELLOW_GPIO_Port, GPIO_LED_2_YELLOW_Pin);
        HAL_GPIO_TogglePin(GPIO_LED_2_GREEN_GPIO_Port, GPIO_LED_2_GREEN_Pin);
        HAL_GPIO_TogglePin(GPIO_LED_2_RED_GPIO_Port, GPIO_LED_2_RED_Pin);

        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}

void UARTTask(void *pvParameters) {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 10;

    uint8_t LPUART1_Buffer[64] = {0};
    uint8_t USART1_Buffer[64] = {0};
    uint8_t USART2_Buffer[64] = {0};
    uint8_t USART3_Buffer[64] = {0};
    uint8_t UART4_Buffer[64] = {0};

    while (true) {
        HAL_UART_Receive(&hlpuart1, LPUART1_Buffer, 64, 80);
        HAL_UART_Transmit(&hlpuart1, LPUART1_Buffer, 64, 10);

        HAL_UART_Receive(&huart1, USART1_Buffer, 64, 80);
        HAL_UART_Transmit(&huart1, USART1_Buffer, 64, 10);

        HAL_UART_Receive(&huart2, USART2_Buffer, 64, 80);
        HAL_UART_Transmit(&huart2, USART2_Buffer, 64, 10);

        HAL_UART_Receive(&huart3, USART3_Buffer, 64, 80);
        HAL_UART_Transmit(&huart3, USART3_Buffer, 64, 10);

        HAL_UART_Receive(&huart4, UART4_Buffer, 64, 80);
        HAL_UART_Transmit(&huart4, UART4_Buffer, 64, 10);

        memset(LPUART1_Buffer, 0, sizeof(LPUART1_Buffer));
        memset(USART1_Buffer, 0, sizeof(USART1_Buffer));
        memset(USART2_Buffer, 0, sizeof(USART2_Buffer));
        memset(USART3_Buffer, 0, sizeof(USART3_Buffer));
        memset(UART4_Buffer, 0, sizeof(UART4_Buffer));

        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}

void SDMMCTask(void *pvParameters) {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 10;

    while (true) {
        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}

void PWMTask(void *pvParameters) {
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 50;

    PWMChannel pwm_T3_C4 = PWMChannel(0, nullptr, &htim3, TIM_CHANNEL_4);
    PWMChannel pwm_T3_C3 = PWMChannel(0, nullptr, &htim3, TIM_CHANNEL_3);
    PWMChannel pwm_T3_C2 = PWMChannel(0, nullptr, &htim3, TIM_CHANNEL_2);
    PWMChannel pwm_T3_C1 = PWMChannel(0, nullptr, &htim3, TIM_CHANNEL_1);
    PWMChannel pwm_T4_C4 = PWMChannel(0, nullptr, &htim4, TIM_CHANNEL_4);
    PWMChannel pwm_T4_C3 = PWMChannel(0, nullptr, &htim4, TIM_CHANNEL_3);
    PWMChannel pwm_T4_C2 = PWMChannel(0, nullptr, &htim4, TIM_CHANNEL_2);
    PWMChannel pwm_T4_C1 = PWMChannel(0, nullptr, &htim4, TIM_CHANNEL_1);

    DSHOTChannel dshot_T1_C2 =
        DSHOTChannel(0, nullptr, &htim1, TIM_CHANNEL_2, TIM_DMA_ID_CC2, TIM_DMA_CC2);
    DSHOTChannel dshot_T1_C1 =
        DSHOTChannel(0, nullptr, &htim1, TIM_CHANNEL_1, TIM_DMA_ID_CC1, TIM_DMA_CC1);
    DSHOTChannel dshot_T5_C4 =
        DSHOTChannel(0, nullptr, &htim5, TIM_CHANNEL_4, TIM_DMA_ID_CC4, TIM_DMA_CC4);
    DSHOTChannel dshot_T5_C3 =
        DSHOTChannel(0, nullptr, &htim5, TIM_CHANNEL_3, TIM_DMA_ID_CC3, TIM_DMA_CC3);
    DSHOTChannel dshot_T5_C2 =
        DSHOTChannel(0, nullptr, &htim5, TIM_CHANNEL_2, TIM_DMA_ID_CC2, TIM_DMA_CC2);
    DSHOTChannel dshot_T5_C1 =
        DSHOTChannel(0, nullptr, &htim5, TIM_CHANNEL_1, TIM_DMA_ID_CC1, TIM_DMA_CC1);
    DSHOTChannel dshot_T16_C1 =
        DSHOTChannel(0, nullptr, &htim16, TIM_CHANNEL_1, TIM_DMA_ID_CC1, TIM_DMA_CC1);
    DSHOTChannel dshot_T17_C1 =
        DSHOTChannel(0, nullptr, &htim17, TIM_CHANNEL_1, TIM_DMA_ID_CC1, TIM_DMA_CC1);

    MotorChannel *motor_channels_[16] = {
        &pwm_T3_C4,   &pwm_T3_C3,   &pwm_T3_C2,    &pwm_T3_C1,    &pwm_T4_C4,   &pwm_T4_C3,
        &pwm_T4_C2,   &pwm_T4_C1,   &dshot_T1_C2,  &dshot_T1_C1,  &dshot_T5_C4, &dshot_T5_C3,
        &dshot_T5_C2, &dshot_T5_C1, &dshot_T16_C1, &dshot_T17_C1,
    };

    for (int i = 0; i < NUM_ACTUATOR_CHANNELS; i++) {
        motor_channels_[i]->setup();
    }

    bool up = true;
    int x = 0;

    while (true) {
        for (int i = 0; i < NUM_ACTUATOR_CHANNELS; i++) {
            motor_channels_[i]->set(x);
        }
        if (x >= 100)
            up = false;
        else if (x <= 0)
            up = true;
        x += up ? 1 : -1;

        vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
    }
}
