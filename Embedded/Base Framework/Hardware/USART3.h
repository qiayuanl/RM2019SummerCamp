#ifndef _USART3_H_
#define _USART3_H_
#include "USART2.h"
extern USART_Receve USART3_Receve_Handler;
void USART3_Init(unsigned long baud);
void USART3_IRQHandler(void);
#endif
