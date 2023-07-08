#include "main.h"

#include <cstring>

#include "FreeRTOS.h"
#include "LOS_Actuators.hpp"
#include "SM.hpp"
#include "cmsis_os2.h"
#include "config.hpp"
#include "task.h"
#include "usart.h"
#include "i2c.h"
#include "spi.h"
#include "adc.h"
#include <cstdio>


 void GPIOTask(void *pvParameters);
 void LEDTask(void *pvParameters);
 void UARTTask(void *pvParameters);
 void SDMMCTask(void *pvParameters);
 void PWMTask(void *pvParameters);


 //dumb ass code i wrote
 void I2CTask(void *pvParameters);
 void SPITask(void *pvParameters);
 void ADCTask(void *pvParameters);

void SMOperationTask(void *pvParameters);
const static auto SM_PERIOD_MS = 5;

int main() {
    losInit();

//     TaskHandle_t SM_handle = NULL;
//     xTaskCreate(SMOperationTask, "SM Thread", 400U, NULL, osPriorityNormal, &SM_handle);

     TaskHandle_t hGPIO = NULL;
     xTaskCreate(GPIOTask, "GPIO", 50U, NULL, osPriorityNormal, &hGPIO);

     TaskHandle_t hLED = NULL;
     xTaskCreate(LEDTask, "LED", 50U, NULL, osPriorityNormal, &hLED);

     TaskHandle_t hUART = NULL;
     xTaskCreate(UARTTask, "UART", 500U, NULL, osPriorityNormal, &hUART);

     TaskHandle_t hSDMMC = NULL;
     xTaskCreate(SDMMCTask, "SDMMC", 100U, NULL, osPriorityNormal, &hSDMMC);

//     TaskHandle_t hI2C = NULL;
//     xTaskCreate(I2CTask, "I2C", 200U, NULL, osPriorityNormal, &hI2C);

     TaskHandle_t hPWM = NULL;
     xTaskCreate(PWMTask, "PWM", 200U, NULL, osPriorityNormal, &hPWM);
//
//     TaskHandle_t hSPI = NULL;
//     xTaskCreate(SPITask, "SPI", 200U, NULL, osPriorityNormal, &hSPI);
//
     TaskHandle_t hADC = NULL;
     xTaskCreate(ADCTask, "ADC", 200U, NULL, osPriorityNormal, &hADC);

     losKernelStart();

    // should not get here bc losInit() starts the scheduler
    while (1) {
    }

    return 0;
}

void I2CTask(void *pvParameters)
{
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 2;

    static const uint8_t DEVICE_ADDRESS = 0x10; //what is this address


    uint8_t I2C1_Buffer[64];
    for (int i = 0; i < 64; i++) {
    	I2C1_Buffer[i] = i;
    }
	uint8_t I2C2_Buffer[64] = {0};
	uint8_t I2C3_Buffer[64] = {0};

	while(true)
	{
	    HAL_I2C_Master_Transmit(&hi2c1,DEVICE_ADDRESS, I2C1_Buffer,64,100);
	    //HAL_I2C_Master_Receive(&hi2c1,DEVICE_ADDRESS, I2C1_Buffer,64,100);

	    HAL_I2C_Master_Transmit(&hi2c2,DEVICE_ADDRESS, I2C2_Buffer,64,100);
	    HAL_I2C_Master_Receive(&hi2c2,DEVICE_ADDRESS,I2C2_Buffer,64,100);


	    HAL_I2C_Master_Transmit(&hi2c3,DEVICE_ADDRESS, I2C3_Buffer,64,100);
	    HAL_I2C_Master_Receive(&hi2c3,DEVICE_ADDRESS, I2C3_Buffer,64,100);

		vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
	}

}

void SPITask(void *pvParameters)
{

	TickType_t xNextWakeTime = xTaskGetTickCount();
	uint16_t frequency = 2;

    uint8_t SPI1_Buffer[64] = {0};
	uint8_t SPI2_Buffer[64] = {0};

	while(true)
		{
	HAL_SPI_Transmit(&hspi1, SPI1_Buffer, 64, 100);
	HAL_SPI_Receive(&hspi1, SPI1_Buffer, 64, 100);

	HAL_SPI_Transmit(&hspi1, SPI2_Buffer, 64, 100);
	HAL_SPI_Receive(&hspi1, SPI2_Buffer, 64, 100);

	vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
		}


}

void ADCTask(void *pvParameters)
{
    TickType_t xNextWakeTime = xTaskGetTickCount();
    uint16_t frequency = 2;

    //4096  2^12 @ 3.3v


    //where do I declare the adc channels
    //like hadc1? adc_hadnletypedef



	while(true)
		{
    HAL_ADC_Start(&hadc2);

    //HAL_GPIO_WritePin(GPIOD, GPIO_8_Pin,GPIO_PIN_SET);//pulling ch11 high (3.3v i think)

    HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
    double raw = HAL_ADC_GetValue(&hadc2);
    char msg[20];
    //raw = raw * (3.3/4096);
    //sprintf(msg, "%f\r\n", raw);
    //it should be printing 3.3v
    uint8_t LPUART1_Buffer[20] = {raw};
    HAL_UART_Transmit(&hlpuart1, LPUART1_Buffer, 20, 10);

	vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
		}
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
         //HAL_GPIO_TogglePin(GPIO_8_GPIO_Port, GPIO_8_Pin);
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
//         HAL_UART_Receive(&hlpuart1, LPUART1_Buffer, 64, 80);
//         HAL_UART_Transmit(&hlpuart1, LPUART1_Buffer, 64, 10);

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

     Los_Actuators actuators = Los_Actuators::getInstance();
     bool up = true;
     int x = 0;

     while (true) {
         for (int i = 0; i < NUM_ACTUATOR_CHANNELS; i++) {
             actuators.set(i, x);
         }
         if (x >= 100)
             up = false;
         else if (x <= 0)
             up = true;
         x += up ? 1 : -1;

         vTaskDelayUntil(&xNextWakeTime, 1000 / frequency);
     }
 }
