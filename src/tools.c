#include "all_headers.h"


extern volatile double freq_t;
extern volatile char dir;



void data_to_char_array(char * _buffer){
    int freq=0;
    //freq = (freq_l +freq_r)/2 * 1000;
    freq = freq_t/2 * 1000;

    _buffer[0] = 'M';
    _buffer[1] =  'D';
    _buffer[2] =  'D';
    _buffer[3] = dir;
    _buffer[4] = freq>>24 & 0xff;
    _buffer[5] = freq>>16 & 0xff;
    _buffer[6] = freq>>8 & 0xff;
    _buffer[7] = freq & 0xff;
    _buffer[8] = 0x00;

}

Device_info dev_info;


void Load_Device_info(){
    uint16_t address = 0x0000;
    uint8_t buffer[20];
    int i=0;
    M95160_Read(address,DEVICE_DATA_SN,buffer);
    for (i=0;i<DEVICE_DATA_SN;i++) dev_info.SN[i] = buffer[i];
    address+=DEVICE_DATA_SN;

    M95160_Read(address,DEVICE_DATA_DATE,buffer);
    for (i=0;i<DEVICE_DATA_DATE;i++) dev_info.Calib_Date[i] = buffer[i];
    address+=DEVICE_DATA_DATE;
    M95160_Read(address,DEVICE_DATA_DATE,buffer);
    for (i=0;i<DEVICE_DATA_DATE;i++) dev_info.Manufacturing_Date[i] = buffer[i];
    address+=DEVICE_DATA_DATE;
    M95160_Read(address,DEVICE_DATA_DATE,buffer);
    for (i=0;i<DEVICE_DATA_DATE;i++) dev_info.Check_Date[i] = buffer[i];
    address+=DEVICE_DATA_DATE;

    M95160_Read(address,DEVICE_DATA_SW_VER,buffer);
    for (i=0;i<DEVICE_DATA_SW_VER;i++) dev_info.SW_ver[i] = buffer[i];
    address+=DEVICE_DATA_SW_VER;

    M95160_Read(address,DEVICE_DATA_PCB_VER,buffer);
    for (i=0;i<DEVICE_DATA_PCB_VER;i++) dev_info.PCB_ver[i] = buffer[i];



};


void Save_Device_Info(Device_info modified){
    int i=0;
    M95160_Write_EN();
    M95160_Write(0x0000,modified.SN,32);

    for (i = 0;i<3234561;i++);
    M95160_Write_EN();
    M95160_Write(32,modified.Check_Date,18);

    M95160_Write_EN();
    M95160_Write(50,"000000000000000000",14);

    for (i = 0;i<3234561;i++);
}
