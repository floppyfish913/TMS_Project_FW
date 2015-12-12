#ifndef _PP_prot_H
#define _PP_prot_H
#include "all_headers.h"

#define DEVICE_DATA_SN  16
#define DEVICE_DATA_SW_VER  5
#define DEVICE_DATA_PCB_VER 5
#define DEVICE_DATA_DATE    8


typedef struct{
    char SN[16];
    char Calib_Date[8];
    char Manufacturing_Date[8];
    char Check_Date[8];
    char SW_ver[5];
    char PCB_ver[5];
} Device_info;


void Load_Device_info();

void data_to_char_array(char * _buffer);
#endif
