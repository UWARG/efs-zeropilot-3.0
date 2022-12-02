#include "main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"
#include "LOS_Actuators.hpp"
#include "LOS_Link.hpp"
#include <cstdio>
#include <string.h>
//#include <stdlib>

void StartBlinkyTest(void * argument);
void StartLosLinkTest(void * argument);
void StartLosLinkTest1(void * argument);
void StartLosLinkTest2(void * argument);


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;
int main()
{
    losInit();
    //
     osThreadAttr_t blinkyTest = {
         .name = "start_blinky",
         .stack_size = 512,
         .priority = osPriorityNormal
     };

     osThreadAttr_t losLinkTest = {
         .name = "start_los_link",
         .stack_size = 512,
         .priority = osPriorityNormal
     };

     osThreadAttr_t losLinkTest1 = {
              .name = "start_los_link1",
              .stack_size = 512,
              .priority = osPriorityNormal
          };

     osThreadAttr_t losLinkTest2 = {
              .name = "start_los_link2",
              .stack_size = 512,
              .priority = osPriorityNormal
          };

     osThreadNew (StartBlinkyTest, NULL, &blinkyTest);
     osThreadNew (StartLosLinkTest, NULL, &losLinkTest);
//     osThreadNew (StartLosLinkTest, NULL, &losLinkTest1);
//     osThreadNew (StartLosLinkTest, NULL, &losLinkTest2);

     losKernelStart();

    //should not get here bec losInit() starts the scheduler
    while(1){
    }

    return 0;
}


void StartBlinkyTest(void * argument)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//        HAL_Delay(250);
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
        osDelay(100);
    }
}

void StartLosLinkTest(void * argument)
{
    for (;;)
    {
//        HAL_Delay(250);
        HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
        LosLinkRx_t testVal = Los_Link::getInstance().getRx(0);

        uint8_t txd_msg_buffer[64] = {0};
//        sprintf((char*)txd_msg_buffer, "\r\n test vals: %d %d %d %d", testVal.rx_channels[0], testVal.rx_channels[1], testVal.rx_channels[2], testVal.rx_channels[3]);
//        HAL_UART_Transmit(&huart1, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
//        HAL_UART_Transmit(&huart2, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
//        HAL_UART_Transmit(&huart3, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
//        HAL_UART_Transmit(&huart5, txd_msg_buffer, strlen((char*)txd_msg_buffer), 1000);
        osDelay(100);
    }

}
void StartLosLinkTest1(void * argument)
{
    for (;;)
    {
//        HAL_Delay(250);
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
//        HAL_Delay(250);
//        LosLinkRx_t testVal = Los_Link::getInstance().getRx(0);
        osDelay(100);
    }

}
void StartLosLinkTest2(void * argument)
{
    for (;;)
    {
//        HAL_Delay(250);
        int x = x+3;
//        HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
//        LosLinkRx_t testVal = Los_Link::getInstance().getRx(0);
        osDelay(100);
    }

}
