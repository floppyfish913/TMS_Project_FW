
#include "all_headers.h"

void Init_M95160_eeprom(){

    int i;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
    /**Init GPIO Pins**/
    GPIO_InitTypeDef io1,io2,io3;

    io1.GPIO_Mode = GPIO_Mode_AF;
    io1.GPIO_Speed = GPIO_Speed_10MHz;
    io1.GPIO_OType = GPIO_OType_PP;
    io1.GPIO_PuPd = GPIO_PuPd_UP;
    //set io pins

    io1.GPIO_Pin= GPIO_Pin_15 | GPIO_Pin_14;
    GPIO_Init(GPIOB,&io1);

    io2 = io1;


    io2.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOD,&io2);

    io3 = io2;

    io3.GPIO_Mode = GPIO_Mode_OUT;
    io3.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&io3);



    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_5);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_5);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_5);
    GPIO_WriteBit(GPIOA,GPIO_Pin_8,0x01);

    /**Init SPI **/
    SPI_InitTypeDef spi;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_DataSize = SPI_DataSize_8b;

    SPI_Init(SPI2,&spi);
    SPI_RxFIFOThresholdConfig(SPI2,SPI_RxFIFOThreshold_QF);
    SPI_Cmd(SPI2,ENABLE);

    /**SPI RX_FIFO_Clear**/
    for(i=0;i<4;i++) SPI_ReceiveData8(SPI2);


 }
void M95160_Write_EN(){
    M95160_Select();
    SPI_SendData8(SPI2,M95xxx_WREN);
    M95160_Discard();
    M95160_DeSelect();
}
void M95160_Write(uint16_t address,uint8_t * data,uint8_t length){
    int i;
    //while (M95160_Read_SR() & M95xxx_WIP);
    M95160_Select();
    SPI_SendData8(SPI2,M95xxx_WRITE);
    SPI_SendData8(SPI2,(address  & 0xff00)>>8);
    SPI_SendData8(SPI2,address & 0x00ff);
    for(i=0;i<length;i++){
        while(SPI_GetTransmissionFIFOStatus(SPI2) > SPI_TransmissionFIFOStatus_HalfFull );
        SPI_SendData8(SPI2,data[i]);
        }
    M95160_Discard();
    M95160_DeSelect();
 }

void M95160_Read(uint16_t address,uint8_t length,uint8_t * rec_buffer){
    int i,j = 0;
    uint8_t temp[64];
    if (length >=50 ) return;

    M95160_Select();

    while(SPI_GetTransmissionFIFOStatus(SPI2) > SPI_TransmissionFIFOStatus_HalfFull);
    SPI_SendData8(SPI2,M95xxx_READ);
    M95160_Discard();

    while(SPI_GetTransmissionFIFOStatus(SPI2) > SPI_TransmissionFIFOStatus_HalfFull);
    SPI_SendData8(SPI2,(address  & 0xff00)>>8);
    M95160_Discard();

    while(SPI_GetTransmissionFIFOStatus(SPI2) > SPI_TransmissionFIFOStatus_HalfFull);
    SPI_SendData8(SPI2,address & 0x00ff);
    M95160_Discard();


    for(i=0;i<length;i++){
        while(SPI_GetTransmissionFIFOStatus(SPI2) > SPI_TransmissionFIFOStatus_HalfFull);
            SPI_SendData8(SPI2,0x00);
        while(SPI_GetReceptionFIFOStatus(SPI2) < SPI_ReceptionFIFOStatus_1QuarterFull);
        rec_buffer[i] = SPI_ReceiveData8(SPI2);
    }


    M95160_DeSelect();
 }

uint8_t M95160_Read_SR(){
    uint8_t result = 0;
    M95160_Select();
    SPI_SendData8(SPI2,M95xxx_RDSR);

    M95160_Discard();

    SPI_SendData8(SPI2,0x00);

    while(SPI_GetReceptionFIFOStatus(SPI2) < SPI_ReceptionFIFOStatus_1QuarterFull);
    while (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE))
        result = SPI_ReceiveData8(SPI2);

    M95160_DeSelect();
    return result;
 }

void M95160_Select(){
    GPIO_WriteBit(GPIOA,GPIO_Pin_8,0x00);

}

void M95160_DeSelect(){
    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY));
    GPIO_WriteBit(GPIOA,GPIO_Pin_8,0x01);
}

void M95160_Discard(){
        while(SPI_GetReceptionFIFOStatus(SPI2) < SPI_ReceptionFIFOStatus_1QuarterFull);
        while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE))
            SPI_ReceiveData8(SPI2);
};

