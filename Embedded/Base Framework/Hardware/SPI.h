#ifndef __spi_H
#define __spi_H
#include "System_Config.h"

#define SPI4_NSS PEout(4)
extern SPI_HandleTypeDef SPI4_Handler;  //SPI¾ä±ú
void SPI4_Init(void);
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI4_ReadWriteByte(u8 TxData);
extern SPI_HandleTypeDef hspi1;
extern void _Error_Handler(char *, int);
void MX_SPI1_Init(void);
#endif
