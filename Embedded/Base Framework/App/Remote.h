#ifndef __BSP_UART_H__
#define __BSP_UART_H__
#define DBUS_HUART huart1 /* for dji remote controler reciever */
#include "System_Config.h"

#define Left_H 1
#define Left_M 3
#define Left_L 2
#define Right_H 1
#define Right_M 3
#define Right_L 2

typedef __packed struct
{
  /* rocker channel information */
  int16_t ch1;
  int16_t ch2;
  int16_t ch3;
  int16_t ch4;
  /* left and right lever information */
  uint8_t sw1;
  uint8_t sw2;
} rc_info_t;

extern rc_info_t Remote;
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream);
void uart_receive_handler(UART_HandleTypeDef *huart);
void Remote_Init(void);
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
#endif

