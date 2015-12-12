#include "all_headers.h"


/***********USER variables***************/
extern volatile UART_MSG msg;

extern volatile message_buffer msg_buffer;

/***************USER FUNCTIONS**********/


//PP protocol writer function

int PP_Write(const char *_ptr, char bytes){
  unsigned int i;


  while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
  USART_SendData(USART1,START_CHAR);

  for(i=0 ; i<bytes ; i++)
  {
     if(_ptr[i] == END_CHAR || _ptr[i]==START_CHAR) {
     while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
     USART_SendData(USART1,ESC_CHAR);
     }
     while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
     USART_SendData(USART1,_ptr[i]);
  }

  while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
  USART_SendData(USART1,END_CHAR);

  return bytes;
}
void PP_Write_withCMD(const char *_ptr, char bytes,char * cmd){
    char sender[32],i=0;
    sender[0] = cmd [0];
    sender[1] = cmd [1];
    sender[2] = cmd [2];
    for(i=0;i<bytes;i++)
        if (i + 3 <32) sender[i+3] = _ptr[i];
    PP_Write(sender,bytes+3);
}

//PP reader function
//Cuts the commands overhead

void PP_Reader(){
	uint8_t i=0,flag=0;

    if (msg.data[msg.length-1] != START_CHAR && msg.length == 1)
        msg.length =0;

    else if(msg.data[msg.length-1] == END_CHAR)
        if(msg.length > 1 && msg.data[msg.length-2]!=ESC_CHAR){
			msg_buffer.length=0;
			//PP message copy
			for(i=1;i<msg.length-1;i++)  //cut off start and end character
			{
				if(msg.data[i] == ESC_CHAR && flag ==0)	flag=1;
				else {
					flag=0;
					if (i<4)
                        msg_buffer.type[i-1] = msg.data[i];
					else
                        msg_buffer.message[msg_buffer.length++] = msg.data[i];
				}

			}
            msg.length = 0;
    }
}

void PP_Acknowledge(){
    PP_Write("ACK",3);
    }
