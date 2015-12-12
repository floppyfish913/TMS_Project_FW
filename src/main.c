/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "all_headers.h"


extern volatile int dir;
extern Device_info dev_info;

int main(void)
{
  int i;

  init_Peripherials();
  Load_Device_info();

  while(1)
  {
    if (dir==1) LED_Color(GREEN);
        else LED_Color(RED);

    MSG_Handler();

  }


}
