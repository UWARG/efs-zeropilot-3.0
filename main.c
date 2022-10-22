#include "main.h"

int main()
{

    //MUST INITIALIZE SYSTEM AND HAL

    while(1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
        HAL_Delay(1000);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
        HAL_Delay(1000);
    }

    return 0;
}
