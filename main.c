#include "main.h"
#include "stm32f4xx_it.h"

int main()
{

    los_init();
    
    while(1)
    {
       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
//        HAL_Delay(1000);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
//        HAL_Delay(1000);

    }

    return 0;
}
