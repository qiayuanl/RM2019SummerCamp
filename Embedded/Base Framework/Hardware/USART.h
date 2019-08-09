#ifndef __usart_H
#define __usart_H
#include "System_Config.h"
#include "Var.h"

typedef struct
{
	unsigned char Receve_Flag;
	unsigned char Buffer[USART2_Receve_Length];
	int Buffer_Counter;
}USART_Receve;
extern USART_Receve USART3_Receve_Handler;
void USART3_Init(unsigned long baud);
void USART3_IRQHandler(void);
extern USART_Receve USART2_Receve_Handler;
extern UART_HandleTypeDef USART2_Handler;
extern UART_HandleTypeDef USART3_Handler; //UART3¾ä±ú
void USART2_init(u32 bound);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void USART2_IRQHandler(void);
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
void USART1_Init(void);
#endif
