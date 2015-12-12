#include "all_headers.h"

void init_Peripherials(){
    clock_config();
    //RCC_DeInit();
    IO_init();
    init_communication();
    config_timing();
    init_IT_routines();
    init_exti();
    Init_M95160_eeprom();

}
void IO_init(){

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);

    GPIO_InitTypeDef red_led;
    red_led.GPIO_Mode=GPIO_Mode_OUT;
    red_led.GPIO_OType=GPIO_OType_PP;
    red_led.GPIO_Pin=GPIO_Pin_2;
    red_led.GPIO_Speed=GPIO_Speed_10MHz;
    red_led.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&red_led);

    GPIO_InitTypeDef green_led;
    green_led.GPIO_Mode=GPIO_Mode_OUT;
    green_led.GPIO_OType=GPIO_OType_PP;
    green_led.GPIO_Pin=GPIO_Pin_13;
    green_led.GPIO_Speed=GPIO_Speed_10MHz;
    green_led.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC,&green_led);

    GPIO_InitTypeDef uart_io;
    uart_io.GPIO_Mode=GPIO_Mode_AF;
    uart_io.GPIO_OType=GPIO_OType_PP;
    uart_io.GPIO_Pin=GPIO_Pin_9 | GPIO_Pin_10;
    uart_io.GPIO_Speed=GPIO_Speed_50MHz;
    uart_io.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&uart_io);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_7);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_7);


    GPIO_InitTypeDef tim2_io;
    tim2_io.GPIO_Mode=GPIO_Mode_AF;
    tim2_io.GPIO_OType=GPIO_OType_PP;
    tim2_io.GPIO_Pin=GPIO_Pin_0;
    tim2_io.GPIO_Speed=GPIO_Speed_50MHz;
    tim2_io.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&tim2_io);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_1);
    //GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_1);
    GPIO_InitTypeDef dir_pin;
    dir_pin.GPIO_Mode=GPIO_Mode_IN;
    dir_pin.GPIO_OType=GPIO_OType_PP;
    dir_pin.GPIO_Pin=GPIO_Pin_1;
    dir_pin.GPIO_Speed=GPIO_Speed_50MHz;
    dir_pin.GPIO_PuPd=GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&dir_pin);
}
void init_communication(){

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    USART_InitTypeDef uart_config;
    uart_config.USART_BaudRate = 115200;
    uart_config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uart_config.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
    uart_config.USART_Parity=USART_Parity_No;
    uart_config.USART_StopBits=USART_StopBits_1;
    uart_config.USART_WordLength=USART_WordLength_8b;
    USART_Init(USART1,&uart_config);
    USART_Cmd(USART1,ENABLE);

    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel =USART1_IRQn;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&nvic);

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

}

void config_timing(){
    TIM_DeInit(TIM2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    TIM_TimeBaseInitTypeDef m_timer;
    m_timer.TIM_CounterMode = TIM_CounterMode_Up;
    m_timer.TIM_Prescaler = 71;
    m_timer.TIM_ClockDivision = 0;
    m_timer.TIM_Period = 100000;
    m_timer.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2,&m_timer);
    TIM_TimeBaseInit(TIM5,&m_timer);

    TIM_ICInitTypeDef ic_init;
    ic_init.TIM_Channel = TIM_Channel_1;
    ic_init.TIM_ICFilter = 0;
    ic_init.TIM_ICPolarity = TIM_ICPolarity_Falling;
    ic_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    ic_init.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_PWMIConfig(TIM2,&ic_init);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);
    TIM_Cmd(TIM2,ENABLE);





      /* Select the TIM2 Input Trigger: TI2FP2 */
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);

  /* Select the slave Mode: Reset Mode */
    TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);

    TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI1,TIM_ICPolarity_Rising,TIM_ICPolarity_Falling);
    TIM_Cmd(TIM5,ENABLE);

    TIM_DeInit(TIM4);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    m_timer.TIM_Prescaler = 719;
    m_timer.TIM_ClockDivision = 0;
    m_timer.TIM_Period = 99;
    m_timer.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4,&m_timer);
    TIM_Cmd(TIM4,ENABLE);
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);



}

void init_IT_routines(){

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    NVIC_InitTypeDef timer;
    timer.NVIC_IRQChannel = TIM2_IRQn;
    timer.NVIC_IRQChannelCmd = ENABLE;
    timer.NVIC_IRQChannelPreemptionPriority = 0;
    timer.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&timer);


    timer.NVIC_IRQChannel = TIM4_IRQn;
    timer.NVIC_IRQChannelCmd = ENABLE;
    timer.NVIC_IRQChannelPreemptionPriority = 0;
    timer.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&timer);
}

void clock_config(){
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    RCC_PLLCmd(DISABLE);
    while(RCC_WaitForHSEStartUp()!=SUCCESS);
    RCC_HCLKConfig(RCC_HCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

}

void init_exti(){

      /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  EXTI_InitTypeDef EXTI_InitStructure;
  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitTypeDef NVIC_InitStructure;
  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);



}
