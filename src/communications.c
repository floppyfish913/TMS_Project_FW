#include "all_headers.h"

/***********USER variables***************/
volatile UART_MSG msg={
    .length=0,
};

volatile message_buffer msg_buffer={
	.length = 0,
    .type = "\000\000\000"
};

extern Device_info dev_info;


/***************USER FUNCTIONS**********/



void MSG_Handler(){
    unsigned char t=0;
    int i;
    if(msg_buffer.type[0]){
        t = msg_buffer.type[0]-'R'+msg_buffer.type[1]+msg_buffer.type[2];


    switch(t){
        /// write command handlers
        case cmd_WSN:
            for(i=0;i<16;i++)
                dev_info.SN[i] = msg_buffer.message[i];
            PP_Acknowledge();
        break;
        case cmd_WMD:
            for(i=0;i<8;i++)
                dev_info.Manufacturing_Date[i] = msg_buffer.message[i];
            PP_Acknowledge();
        break;
        case cmd_WCD:
            for(i=0;i<8;i++)
                dev_info.Calib_Date[i] = msg_buffer.message[i];
            PP_Acknowledge();
        break;
        case cmd_WED:
            for(i=0;i<8;i++)
                dev_info.Check_Date[i] = msg_buffer.message[i];
            PP_Acknowledge();
        break;
        case cmd_WPC:
            for(i=0;i<5;i++)
                dev_info.PCB_ver[i] = msg_buffer.message[i];
            PP_Acknowledge();
        break;
        case cmd_WSV:
            for(i=0;i<5;i++)
                dev_info.SW_ver[i] = msg_buffer.message[i];
            PP_Acknowledge();
        break;
        case cmd_SUD:
            Save_Device_Info(dev_info);
            PP_Acknowledge();
        break;

        ///read command handlers
        case cmd_FDI:
            PP_Acknowledge();
            PP_Write("\r\n",2);
            PP_Write(dev_info.SN,16);
            PP_Write("\r\n",2);
            PP_Write(dev_info.Manufacturing_Date,8);
            PP_Write("\r\n",2);
            PP_Write(dev_info.Calib_Date,8);
            PP_Write("\r\n",2);
            PP_Write(dev_info.Check_Date,8);
            PP_Write("\r\n",2);
            PP_Write(dev_info.SW_ver,5);
            PP_Write("\r\n",2);
            PP_Write(dev_info.PCB_ver,5);
            PP_Write("\r\n",2);
        break;

        case cmd_RSN:
            PP_Write_withCMD(dev_info.SN,16,"RSN");
        break;
        case cmd_RMD:
            PP_Write_withCMD(dev_info.Manufacturing_Date,8,"RMD");
        break;
        case cmd_RCD:
            PP_Write_withCMD(dev_info.Calib_Date,8,"RCD");
        break;
        case cmd_RED:
            PP_Write_withCMD(dev_info.Check_Date,8,"RED");
        break;
        case cmd_RSV:
            PP_Write_withCMD(dev_info.SW_ver,5,"RSV");
        break;
        case cmd_RPC:
            PP_Write_withCMD(dev_info.PCB_ver,5,"RPC");
        break;





        ///control handlers

        case cmd_RST:
            PP_Acknowledge();
            NVIC_SystemReset();
        break;

        default:
            PP_Write("INV",3);
        break;

        }
        msg_buffer.length=0;
        msg_buffer.type[0] = 0x00;
    }
}



int send_data(int data){
    USART_SendData(USART1,data & 0xff);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC));
    USART_SendData(USART1,data>>8 & 0xff);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC));
    USART_SendData(USART1,data>>16 & 0xff);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC));
    USART_SendData(USART1,data>>24 & 0xff);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC));
    return 1;
}


volatile char temp[345];

void USART1_IRQHandler(){
    if(USART_GetITStatus(USART1,USART_IT_RXNE)){
        msg.data[msg.length] = USART_ReceiveData(USART1);
        //temp[msg.length]= USART_ReceiveData(USART1);
        msg.length++;
        PP_Reader();
    }
    USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}
