#include "stm32f37x_conf.h"
#include "gpio_p.h"
void LED_Color(char color){

    switch (color){
        default :
            GPIO_ResetBits(GPIOA,GPIO_Pin_2);
            GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        break;
        case GREEN :
            GPIO_ResetBits(GPIOA,GPIO_Pin_2);
            GPIO_SetBits(GPIOC,GPIO_Pin_13);
        break;
        case ORANGE :
            GPIO_SetBits(GPIOA,GPIO_Pin_2);
            GPIO_SetBits(GPIOC,GPIO_Pin_13);
        break;
        case RED :
            GPIO_SetBits(GPIOA,GPIO_Pin_2);
            GPIO_ResetBits(GPIOC,GPIO_Pin_13);
        break;
    }
}
