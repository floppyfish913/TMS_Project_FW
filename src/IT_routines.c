#include "all_headers.h"
#define MEASURE_STARTED 0x1
#define MEASURE_ENDED 0x00
volatile char dir=0;
char msg_buffer[100];
volatile char new_data = 0x00;
volatile double freq_t = 0;
volatile char status = 0x00;
void TIM2_IRQHandler(){
int data = 0;
    if (TIM_GetFlagStatus(TIM2,TIM_FLAG_CC2)) {
            data = TIM_GetCapture2(TIM2);
        if(data > 250)
                freq_t =1000000 / (double)data;

        if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)) dir=1;
          else dir=-1;
    }
    if (TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)) status = MEASURE_ENDED;

    TIM_ClearITPendingBit(TIM2,TIM_IT_CC2 | TIM_IT_Update);
}

void TIM4_IRQHandler(){
    if (status) {
        data_to_char_array(msg_buffer);
        PP_Write(msg_buffer, 8);
    }
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}

void EXTI0_IRQHandler(){

    //start measure
    TIM_Cmd(TIM2,ENABLE);
    status = MEASURE_STARTED;
    EXTI_ClearITPendingBit(EXTI_Line0);

}
