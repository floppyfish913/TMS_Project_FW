#ifndef PP_prot_H
#define PP_prot_H
#include "stm32f37x_conf.h"


#define START_CHAR 0x5b
#define END_CHAR 0x5d
#define ESC_CHAR 0x5c

int PP_Write(const char *_ptr, char bytes);


#endif
