#ifndef _M95160_eeprom_H
#define _M95160_eeprom_H
#include "all_headers.h"


#define	M95xxx_WREN	0x06
#define	M95xxx_WRDI	0x04
#define	M95xxx_RDSR	0x05
#define	M95xxx_WRSR	0x01
#define	M95xxx_READ	0x03
#define	M95xxx_WRITE    0x02

///Internal signals from SR

#define M95xxx_WIP 0x01
#define M95xxx_WEL 0x02



void Init_M95160_eeprom();
void M95160_Write_EN();
void M95160_Write(uint16_t address,uint8_t * data,uint8_t length);
void M95160_Read(uint16_t address,uint8_t length,uint8_t * rec_buffer);
uint8_t M95160_Read_SR();
void M95160_Select();
void M95160_DeSelect();
void M95160_Discard();
#endif
